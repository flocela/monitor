#ifndef PROCESSOR_FACTORY_H
#define PROCESSOR_FACTORY_H

#include "processor.h"
class ProcessorFactory {
    public:
        Processor createProcessor(int type, int time_delta);
};

#endif