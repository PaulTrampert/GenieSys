//
// Created by paul.trampert on 12/6/2020.
//

#include <GenieSys/CpuOperations/CpuOperation.h>
#include <GenieSys/CpuOperations/Abcd.h>
#include <GenieSys/CpuOperations/Add.h>
#include <algorithm>
#include <GenieSys/CpuOperations/ORItoCCR.h>
#include <GenieSys/CpuOperations/ORItoSR.h>
#include <GenieSys/CpuOperations/ANDI.h>
#include <GenieSys/CpuOperations/ADDI.h>
#include <GenieSys/CpuOperations/SUBI.h>
#include <GenieSys/CpuOperations/ANDItoCCR.h>
#include <GenieSys/CpuOperations/ANDItoSR.h>
#include <GenieSys/CpuOperations/Nop.h>
#include <GenieSys/CpuOperations/ORI.h>
#include <GenieSys/CpuOperations/EORItoCCR.h>
#include <GenieSys/CpuOperations/EORItoSR.h>
#include <GenieSys/CpuOperations/EORI.h>
#include <GenieSys/CpuOperations/CMPI.h>
#include <GenieSys/CpuOperations/BCHG.h>
#include <GenieSys/CpuOperations/BTST.h>
#include <GenieSys/CpuOperations/BCLR.h>
#include <GenieSys/CpuOperations/BSET.h>
#include <GenieSys/CpuOperations/MOVEP.h>
#include <GenieSys/CpuOperations/MOVEA.h>
#include <GenieSys/CpuOperations/MOVE.h>
#include <GenieSys/CpuOperations/MOVEfromSR.h>
#include <GenieSys/CpuOperations/MOVEtoCCR.h>

CpuOperation::CpuOperation(M68kCpu *cpu, Bus *bus) {
    this->cpu = cpu;
    this->bus = bus;
}

std::vector<std::shared_ptr<CpuOperation>> getOperations(M68kCpu *cpu, Bus *bus) {
    std::vector<std::shared_ptr<CpuOperation>> operations{
            std::shared_ptr<CpuOperation>(new Abcd(cpu, bus)),
            std::shared_ptr<CpuOperation>(new Add(cpu, bus)),
            std::shared_ptr<CpuOperation>(new ADDI(cpu, bus)),
            std::shared_ptr<CpuOperation>(new ANDI(cpu, bus)),
            std::shared_ptr<CpuOperation>(new ANDItoCCR(cpu, bus)),
            std::shared_ptr<CpuOperation>(new ANDItoSR(cpu, bus)),
            std::shared_ptr<CpuOperation>(new BCHG(cpu, bus)),
            std::shared_ptr<CpuOperation>(new BCLR(cpu, bus)),
            std::shared_ptr<CpuOperation>(new BSET(cpu, bus)),
            std::shared_ptr<CpuOperation>(new BTST(cpu, bus)),
            std::shared_ptr<CpuOperation>(new CMPI(cpu, bus)),
            std::shared_ptr<CpuOperation>(new EORI(cpu, bus)),
            std::shared_ptr<CpuOperation>(new EORItoCCR(cpu, bus)),
            std::shared_ptr<CpuOperation>(new EORItoSR(cpu, bus)),
            std::shared_ptr<CpuOperation>(new MOVE(cpu, bus)),
            std::shared_ptr<CpuOperation>(new MOVEA(cpu, bus)),
            std::shared_ptr<CpuOperation>(new MOVEfromSR(cpu, bus)),
            std::shared_ptr<CpuOperation>(new MOVEtoCCR(cpu, bus)),
            std::shared_ptr<CpuOperation>(new MOVEP(cpu, bus)),
            std::shared_ptr<CpuOperation>(new Nop(cpu, bus)),
            std::shared_ptr<CpuOperation>(new ORI(cpu, bus)),
            std::shared_ptr<CpuOperation>(new ORItoCCR(cpu, bus)),
            std::shared_ptr<CpuOperation>(new ORItoSR(cpu, bus)),
            std::shared_ptr<CpuOperation>(new SUBI(cpu, bus))
    };

    std::sort(operations.begin(), operations.end(), compare);
    return operations;
}

bool compare(const std::shared_ptr<CpuOperation>& a, const std::shared_ptr<CpuOperation>& b) {
    return a->getSpecificity() > b->getSpecificity();
}
