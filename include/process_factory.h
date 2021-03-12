#ifndef PROCESS_FACTORY_H
#define PROCESS_FACTORY_H

#include "process.h"
#include <memory>

class ProcessFactory {
    public:
        std::unique_ptr<Process> createProcess(int pid, int type);
};

#endif