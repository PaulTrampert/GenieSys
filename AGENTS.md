# GenieSys

A Sega Genesis emulator written in C++20. The bulk of the work so far is a Motorola
68000 (M68k) CPU emulator with a built-in disassembler. Everything is unit tested with
GoogleTest/GoogleMock.

## Build and test

```sh
cmake -S . -B cmake-build
cmake --build cmake-build -j$(nproc)
cd cmake-build && ctest
```

Targets:

* `GenieSys` — static library holding essentially all the source (`src/`, headers in `include/GenieSys/`).
* `GenieSysEmu` — the emulator executable (`app/main.cpp`; currently a stub).
* `testGenieSys` — the GoogleTest suite (`test/`). Registered with CTest via `gtest_discover_tests`,
  so each `TEST_P` case shows up as its own CTest test (~800 and counting).

`ENABLE_DISASSEMBLY` (default `ON`) defines `DISASSEMBLE`, which makes `M68kCpu::tick()`
print the disassembly of each instruction before executing it.

GoogleTest v1.17.0 is pulled in by `FetchContent` at configure time, so the first configure
needs network access.

### Known gotcha: GCC 15+ / libstdc++ 15+

Several headers (starting with `include/GenieSys/CpuOperations/CpuOperation.h`) use
`uint8_t`/`uint16_t` without including `<cstdint>`, relying on it arriving transitively.
Newer libstdc++ dropped that transitive include, so a build with GCC 16 fails with
`'uint8_t' does not name a type`. CI (`ubuntu-latest`, `windows-latest`) still uses older
toolchains and passes. To build locally on a bleeding-edge toolchain without touching the
source, force the include:

```sh
cmake -S . -B cmake-build -DCMAKE_CXX_FLAGS="-include cstdint"
```

If you add `#include <cstdint>` to a header you touch anyway, that is a welcome fix — just
don't turn it into a sweeping unrelated change.

## Architecture

```
Bus  <->  M68kCpu  ->  opTable[65536]  ->  CpuOperation  ->  AddressingMode -> AddressingResult
```

* **`Bus`** (`include/GenieSys/Bus.h`) — 72KB of RAM plus byte/word/long read and write.
  Owns the run thread. All memory access from operations goes through it.
* **`M68kCpu`** (`include/GenieSys/M68kCpu.h`) — registers, PC, SR/CCR, stack helpers,
  `trap()`, `testConditionCode()`, and the eight addressing modes. Nearly every method is
  `virtual` specifically so `test/MockCpu.h` can mock it.
* **Opcode dispatch** — `M68kCpu` builds a flat 65536-entry `opTable` at construction and on
  `ConnectBus()`. It starts filled with `NOP`, then every operation returned by
  `getOperations()` claims each opcode in its `getOpcodes()` list, last write winning.
  `getSpecificity()` returns the count of *variable* (non-fixed) bits in the encoding,
  computed by summing the widths of the operation's `BitMask` fields — so the value is
  inversely named: a **higher** number means a **less** specific instruction.
  `getOperations()` sorts descending by that value, which installs the broadest operations
  first and lets narrower ones overwrite them. Net effect: **the operation with the fewest
  variable bits wins a contested opcode.** `NOP` and `ILLEGAL` both return `0` (fully fixed
  opcode words) and so override broad operations like `MOVE` (14 variable bits) wherever
  their encodings collide. **Getting `getSpecificity()` wrong silently steals or loses
  opcodes.**
* **`CpuOperation`** (`include/GenieSys/CpuOperations/CpuOperation.h`) — one class per M68k
  instruction. Four virtuals: `execute(opWord)` returns the cycle count, `getOpcodes()`
  returns every opcode word the instruction claims, `getSpecificity()`, and
  `disassemble(opWord)` returns the assembly text.
* **`AddressingMode` / `AddressingResult`** (`include/GenieSys/AddressingModes/`) — the eight
  effective-address modes. `getData(regAddr, size)` returns an `AddressingResult` that carries
  the fetched bytes, the resolved address, the EA cycle cost (`getCycles()`), and a `write()`
  back to the destination. Modes are looked up by `cpu->getAddressingMode(eaMode)` and each
  class exposes a `static const uint8_t MODE_ID`; use the constant, never a bare literal.
  Note mode ids are reused across the two families — e.g. `DataRegisterDirectMode::MODE_ID`
  and `AbsoluteShortAddressingMode::MODE_ID` are both `0b000` — because mode `0b111`
  (`ProgramCounterAddressingMode`) dispatches on the register field.

### Shared helpers

* **`BitMask<T>`** (`include/GenieSys/BitMask.h`) — constructed as `BitMask<uint16_t>(highBit, width)`
  (optionally with min/max), then `apply(opWord)` extracts a field and `compose(base, value)`
  inserts one. This is how every operation decodes its opcode word. Restricting the valid
  range (e.g. `BitMask<uint16_t>(7, 2, 0, 2)`) keeps `getPossibleOpcodes` from generating
  encodings the instruction does not own.
* **`getPossibleOpcodes`** (`include/GenieSys/getPossibleOpcodes.h`) — expands a base opcode
  across a list of `BitMask` fields into every concrete opcode. This is what `getOpcodes()`
  is built on.
* **`getCcrFlags.h`** — `getAdditionCcrFlags`, `getSubtractionCcrFlags`, `getNegCcrFlags`,
  `getNegxCcrFlags`. Prefer these over hand-rolling flag logic.
* **`signExtend.h`**, **`numberUtils.h`**, **`ConditionCodes.h`** (`CC_*` macros and
  `getConditionCodeName`), **`enums.h`** (`CCR_*`, `DATA_SIZE`, `TRAP_VECTORS`, …),
  **`TrapException`**, **`RegisterList`** (MOVEM register masks), **`ExtensionWord`**.

## Adding a new instruction

The `ROd` commit (`git show 24c910d`) is a good complete example. Every new instruction
touches the same six places:

1. `include/GenieSys/CpuOperations/<MNEMONIC>.h` — subclass `CpuOperation`, with the
   `BitMask` fields for the encoding as private members.
2. `src/CpuOperations/<MNEMONIC>.cpp` — implement the four virtuals.
3. `src/CMakeLists.txt` — add the `.cpp` to the `GenieSys` target.
4. `src/CpuOperations/CpuOperation.cpp` — add the `#include` and a
   `std::shared_ptr<GenieSys::CpuOperation>(new <MNEMONIC>(cpu, bus))` entry to the
   `getOperations()` list (kept roughly alphabetical).
5. `test/CpuOperations/<MNEMONIC>Test.cpp` — tests.
6. `test/CMakeLists.txt` — add the test `.cpp` to the `testGenieSys` target.

Forgetting step 3, 4, or 6 fails quietly: the code compiles but the instruction is never
dispatched or never tested.

Cycle counts and flag effects must match the reference manuals in `docs/` — cite the page
in a doc comment on the class, the way the existing operations do (e.g. "Documented on page
4-113 of docs/68KPM.pdf"). `docs/README.md` also links an instruction-timings table and an
exceptions reference.

## Test conventions

* Tests are almost always **parameterized** (`TEST_P` over a `struct <OP>TestParams`),
  with a designated-initializer list of cases in `INSTANTIATE_TEST_SUITE_P` and a name
  generator lambda returning `info.param.testName`. Each case usually asserts both the
  execution result (registers, CCR, cycles, write calls) and the expected disassembly string.
* Operations are tested in isolation against `test/MockCpu.h`, `test/MockBus.h`, and
  `test/MockAddressingMode.h`. Because `AddressingMode::getData` returns a
  `std::unique_ptr`, `MockAddressingMode` mocks a raw-pointer `getDataProxy` instead and
  wraps it; set expectations on `getDataProxy`, and call
  `testing::Mock::AllowLeak(addressingResult)` for the result object.
* Integration-style CPU tests live in `test/M68kCpuTests/` and use the real `M68kCpu`
  (see `WithM68kCpu.h`).
* `test/GetPossibleOpcodesTest.cpp` and `test/BitMaskTest.cpp` cover the decoding helpers.

## Conventions

* C++20, four-space indent, `GenieSys` namespace for everything,
  headers use `#pragma once`.
* Implementation files qualify definitions fully (`uint8_t GenieSys::ROd::execute(...)`)
  rather than reopening the namespace.
* Public headers carry Doxygen-style `/** ... */` comments; the existing files lead with a
  `// Created by ...` banner.
* Anything an operation calls on `M68kCpu`, `Bus`, or `AddressingMode` should be `virtual`
  so it can be mocked — if you add a method that operations use, add it to the matching
  mock in `test/` too.
* `cmake-build*/` and `build/` are gitignored; never commit build output.

## CI

`.github/workflows/cmake.yml` builds and runs `ctest` on `ubuntu-latest` and
`windows-latest` for every push, in `Release`, and uploads any `*.log` files as artifacts.
