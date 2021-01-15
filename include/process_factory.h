#ifndef PROCESS_FACTORY_H
#define PROCESS_FACTORY_H

#include "process.h"

class ProcessFactory {
    public:
        Process createProcess(int pid);
};

#endif