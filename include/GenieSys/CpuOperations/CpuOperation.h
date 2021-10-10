//
// Created by paul.trampert on 12/3/2020.
//

#pragma once
#include <vector>
#include <string>
#include <memory>


namespace GenieSys {
    class Bus;
    class M68kCpu;

    class CpuOperation {
    protected:
        GenieSys::Bus* bus;
        GenieSys::M68kCpu* cpu;

    public:
        CpuOperation(GenieSys::M68kCpu* cpu, GenieSys::Bus* bus);
        virtual ~CpuOperation() = default;
        virtual uint8_t execute(uint16_t opWord) = 0;
        virtual std::vector<uint16_t> getOpcodes() = 0;
        virtual uint8_t getSpecificity() = 0;
        virtual std::string disassemble(uint16_t opWord) = 0;
    };

    std::vector<std::shared_ptr<GenieSys::CpuOperation>> getOperations(GenieSys::M68kCpu *cpu, GenieSys::Bus *bus);
    bool compare(const std::shared_ptr<GenieSys::CpuOperation>& a, const std::shared_ptr<GenieSys::CpuOperation>& b);


}
