//
// Created by pault on 12/29/2021.
//

#include "GenieSys/M68kCpu.h"
#include "../MockBus.h"


class WithM68KCpu {
public:
    GenieSys::M68kCpu * subject;
    GenieSys::MockBus bus;

    WithM68KCpu() {
        subject = new GenieSys::M68kCpu();
        subject->ConnectBus(&bus);
    }

    ~WithM68KCpu() {
        delete subject;
    }
};