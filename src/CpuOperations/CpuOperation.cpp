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
#include <GenieSys/CpuOperations/MOVEtoSR.h>
#include <GenieSys/CpuOperations/NEGX.h>
#include <GenieSys/CpuOperations/CLR.h>
#include <GenieSys/CpuOperations/NEG.h>
#include <GenieSys/CpuOperations/NOT.h>



GenieSys::CpuOperation::CpuOperation(GenieSys::M68kCpu *cpu, GenieSys::Bus *bus) {
    this->cpu = cpu;
    this->bus = bus;
}

std::vector<std::shared_ptr<GenieSys::CpuOperation>> GenieSys::getOperations(GenieSys::M68kCpu *cpu, GenieSys::Bus *bus) {
    std::vector<std::shared_ptr<GenieSys::CpuOperation>> operations{
            std::shared_ptr<GenieSys::CpuOperation>(new Abcd(cpu, bus)),
            std::shared_ptr<GenieSys::CpuOperation>(new Add(cpu, bus)),
            std::shared_ptr<GenieSys::CpuOperation>(new ADDI(cpu, bus)),
            std::shared_ptr<GenieSys::CpuOperation>(new ANDI(cpu, bus)),
            std::shared_ptr<GenieSys::CpuOperation>(new ANDItoCCR(cpu, bus)),
            std::shared_ptr<GenieSys::CpuOperation>(new ANDItoSR(cpu, bus)),
            std::shared_ptr<GenieSys::CpuOperation>(new BCHG(cpu, bus)),
            std::shared_ptr<GenieSys::CpuOperation>(new BCLR(cpu, bus)),
            std::shared_ptr<GenieSys::CpuOperation>(new BSET(cpu, bus)),
            std::shared_ptr<GenieSys::CpuOperation>(new BTST(cpu, bus)),
            std::shared_ptr<GenieSys::CpuOperation>(new CLR(cpu, bus)),
            std::shared_ptr<GenieSys::CpuOperation>(new CMPI(cpu, bus)),
            std::shared_ptr<GenieSys::CpuOperation>(new EORI(cpu, bus)),
            std::shared_ptr<GenieSys::CpuOperation>(new EORItoCCR(cpu, bus)),
            std::shared_ptr<GenieSys::CpuOperation>(new EORItoSR(cpu, bus)),
            std::shared_ptr<GenieSys::CpuOperation>(new MOVE(cpu, bus)),
            std::shared_ptr<GenieSys::CpuOperation>(new MOVEA(cpu, bus)),
            std::shared_ptr<GenieSys::CpuOperation>(new MOVEfromSR(cpu, bus)),
            std::shared_ptr<GenieSys::CpuOperation>(new MOVEtoCCR(cpu, bus)),
            std::shared_ptr<GenieSys::CpuOperation>(new MOVEtoSR(cpu, bus)),
            std::shared_ptr<GenieSys::CpuOperation>(new MOVEP(cpu, bus)),
            std::shared_ptr<GenieSys::CpuOperation>(new NEG(cpu, bus)),
            std::shared_ptr<GenieSys::CpuOperation>(new NEGX(cpu, bus)),
            std::shared_ptr<GenieSys::CpuOperation>(new Nop(cpu, bus)),
            std::shared_ptr<GenieSys::CpuOperation>(new NOT(cpu, bus)),
            std::shared_ptr<GenieSys::CpuOperation>(new ORI(cpu, bus)),
            std::shared_ptr<GenieSys::CpuOperation>(new ORItoCCR(cpu, bus)),
            std::shared_ptr<GenieSys::CpuOperation>(new ORItoSR(cpu, bus)),
            std::shared_ptr<GenieSys::CpuOperation>(new SUBI(cpu, bus))
    };

    std::sort(operations.begin(), operations.end(), GenieSys::compare);
    return operations;
}

bool GenieSys::compare(const std::shared_ptr<GenieSys::CpuOperation>& a, const std::shared_ptr<GenieSys::CpuOperation>& b) {
    return a->getSpecificity() > b->getSpecificity();
}