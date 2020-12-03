# GenieSys
![CMake](https://github.com/PaulTrampert/GenieSys/workflows/CMake/badge.svg?branch=main)

Sega Genesis emulator. Current state of the project is "In Development." Will flesh out documentation as I go.

## Building
### Requirements
* git (duh)
* CMake 3.16+
* Windows
  * Requires Visual C/C++ toolchain (MSVC) to be installed
* Linux
  * Requires GNU C/C++ toolchain (g++, gcc, and friends) to be installed

### Targets
* **GenieSys** - This is the main executable. Will grow into the emulator itself.
* **libGenieSys** - Most of the source is built by this target.
* **testGenieSys** - Unit tests. Depends on gtest and gmock.

### Using CLion
* Open the project directory as a CMake project. CLion should load everything up just fine.

### Command line (all platforms)
```
cmake -S . -B cmake-build
cmake --build cmake-build
```

## Running Tests
```
cmake -S . -B cmake-build
cmake --build cmake-build
cd cmake-build
ctest
```
