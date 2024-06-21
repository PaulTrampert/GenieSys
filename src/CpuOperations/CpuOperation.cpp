//
// Created by paul.trampert on 12/6/2020.
//

#include <GenieSys/CpuOperations/CpuOperation.h>
#include <GenieSys/CpuOperations/ABCD.h>
#include <GenieSys/CpuOperations/ADD.h>
#include <algorithm>
#include <GenieSys/CpuOperations/ORItoCCR.h>
#include <GenieSys/CpuOperations/ORItoSR.h>
#include <GenieSys/CpuOperations/ANDI.h>
#include <GenieSys/CpuOperations/ADDI.h>
#include <GenieSys/CpuOperations/SUBI.h>
#include <GenieSys/CpuOperations/ANDItoCCR.h>
#include <GenieSys/CpuOperations/ANDItoSR.h>
#include <GenieSys/CpuOperations/NOP.h>
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
#include <GenieSys/CpuOperations/PEA.h>
#include <GenieSys/CpuOperations/ILLEGAL.h>
#include <GenieSys/CpuOperations/TAS.h>
#include <GenieSys/CpuOperations/TST.h>
#include <GenieSys/CpuOperations/TRAP.h>
#include <GenieSys/CpuOperations/LINK.h>
#include <GenieSys/CpuOperations/UNLK.h>
#include <GenieSys/CpuOperations/MOVE_USP.h>
#include <GenieSys/CpuOperations/RESET.h>
#include <GenieSys/CpuOperations/STOP.h>
#include <GenieSys/CpuOperations/RTE.h>
#include <GenieSys/CpuOperations/RTS.h>
#include <GenieSys/CpuOperations/TRAPV.h>
#include <GenieSys/CpuOperations/RTR.h>
#include <GenieSys/CpuOperations/JSR.h>
#include <GenieSys/CpuOperations/JMP.h>
#include <GenieSys/CpuOperations/Bcc.h>
#include <GenieSys/CpuOperations/BSR.h>
#include <GenieSys/CpuOperations/EXT.h>
#include <GenieSys/CpuOperations/MOVEM.h>
#include <GenieSys/CpuOperations/MOVEQ.h>
#include <GenieSys/CpuOperations/NBCD.h>
#include <GenieSys/CpuOperations/SWAP.h>
#include <GenieSys/CpuOperations/BRA.h>


GenieSys::CpuOperation::CpuOperation(GenieSys::M68kCpu *cpu, GenieSys::Bus *bus) {
    this->cpu = cpu;
    this->bus = bus;
}

std::vector<std::shared_ptr<GenieSys::CpuOperation>> GenieSys::getOperations(GenieSys::M68kCpu *cpu, GenieSys::Bus *bus) {
    std::vector<std::shared_ptr<GenieSys::CpuOperation>> operations{
            std::shared_ptr<GenieSys::CpuOperation>(new ABCD(cpu, bus)),
            std::shared_ptr<GenieSys::CpuOperation>(new ADD(cpu, bus)),
            std::shared_ptr<GenieSys::CpuOperation>(new ADDI(cpu, bus)),
            std::shared_ptr<GenieSys::CpuOperation>(new ANDI(cpu, bus)),
            std::shared_ptr<GenieSys::CpuOperation>(new ANDItoCCR(cpu, bus)),
            std::shared_ptr<GenieSys::CpuOperation>(new ANDItoSR(cpu, bus)),
            std::shared_ptr<GenieSys::CpuOperation>(new Bcc(cpu, bus)),
            std::shared_ptr<GenieSys::CpuOperation>(new BCHG(cpu, bus)),
            std::shared_ptr<GenieSys::CpuOperation>(new BCLR(cpu, bus)),
            std::shared_ptr<GenieSys::CpuOperation>(new BRA(cpu, bus)),
            std::shared_ptr<GenieSys::CpuOperation>(new BSET(cpu, bus)),
            std::shared_ptr<GenieSys::CpuOperation>(new BSR(cpu, bus)),
            std::shared_ptr<GenieSys::CpuOperation>(new BTST(cpu, bus)),
            std::shared_ptr<GenieSys::CpuOperation>(new CLR(cpu, bus)),
            std::shared_ptr<GenieSys::CpuOperation>(new CMPI(cpu, bus)),
            std::shared_ptr<GenieSys::CpuOperation>(new EORI(cpu, bus)),
            std::shared_ptr<GenieSys::CpuOperation>(new EORItoCCR(cpu, bus)),
            std::shared_ptr<GenieSys::CpuOperation>(new EORItoSR(cpu, bus)),
            std::shared_ptr<GenieSys::CpuOperation>(new EXT(cpu, bus)),
            std::shared_ptr<GenieSys::CpuOperation>(new ILLEGAL(cpu, bus)),
            std::shared_ptr<GenieSys::CpuOperation>(new JMP(cpu, bus)),
            std::shared_ptr<GenieSys::CpuOperation>(new JSR(cpu, bus)),
            std::shared_ptr<GenieSys::CpuOperation>(new LINK(cpu, bus)),
            std::shared_ptr<GenieSys::CpuOperation>(new MOVE(cpu, bus)),
            std::shared_ptr<GenieSys::CpuOperation>(new MOVE_USP(cpu, bus)),
            std::shared_ptr<GenieSys::CpuOperation>(new MOVEA(cpu, bus)),
            std::shared_ptr<GenieSys::CpuOperation>(new MOVEfromSR(cpu, bus)),
            std::shared_ptr<GenieSys::CpuOperation>(new MOVEM(cpu, bus)),
            std::shared_ptr<GenieSys::CpuOperation>(new MOVEP(cpu, bus)),
            std::shared_ptr<GenieSys::CpuOperation>(new MOVEQ(cpu, bus)),
            std::shared_ptr<GenieSys::CpuOperation>(new MOVEtoCCR(cpu, bus)),
            std::shared_ptr<GenieSys::CpuOperation>(new MOVEtoSR(cpu, bus)),
            std::shared_ptr<GenieSys::CpuOperation>(new NBCD(cpu, bus)),
            std::shared_ptr<GenieSys::CpuOperation>(new NEG(cpu, bus)),
            std::shared_ptr<GenieSys::CpuOperation>(new NEGX(cpu, bus)),
            std::shared_ptr<GenieSys::CpuOperation>(new NOP(cpu, bus)),
            std::shared_ptr<GenieSys::CpuOperation>(new NOT(cpu, bus)),
            std::shared_ptr<GenieSys::CpuOperation>(new ORI(cpu, bus)),
            std::shared_ptr<GenieSys::CpuOperation>(new ORItoCCR(cpu, bus)),
            std::shared_ptr<GenieSys::CpuOperation>(new ORItoSR(cpu, bus)),
            std::shared_ptr<GenieSys::CpuOperation>(new PEA(cpu, bus)),
            std::shared_ptr<GenieSys::CpuOperation>(new RESET(cpu, bus)),
            std::shared_ptr<GenieSys::CpuOperation>(new RTE(cpu, bus)),
            std::shared_ptr<GenieSys::CpuOperation>(new RTR(cpu, bus)),
            std::shared_ptr<GenieSys::CpuOperation>(new RTS(cpu, bus)),
            std::shared_ptr<GenieSys::CpuOperation>(new STOP(cpu, bus)),
            std::shared_ptr<GenieSys::CpuOperation>(new SUBI(cpu, bus)),
            std::shared_ptr<GenieSys::CpuOperation>(new SWAP(cpu, bus)),
            std::shared_ptr<GenieSys::CpuOperation>(new TAS(cpu, bus)),
            std::shared_ptr<GenieSys::CpuOperation>(new TRAP(cpu, bus)),
            std::shared_ptr<GenieSys::CpuOperation>(new TRAPV(cpu, bus)),
            std::shared_ptr<GenieSys::CpuOperation>(new TST(cpu, bus)),
            std::shared_ptr<GenieSys::CpuOperation>(new UNLK(cpu, bus))
    };

    std::sort(operations.begin(), operations.end(), GenieSys::compareOps);
    return operations;
}

bool GenieSys::compareOps(const std::shared_ptr<GenieSys::CpuOperation>& a, const std::shared_ptr<GenieSys::CpuOperation>& b) {
    return a->getSpecificity() > b->getSpecificity();
}