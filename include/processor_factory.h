#ifndef PROCESSOR_FACTORY_H
#define PROCESSOR_FACTORY_H

#include <memory>
#include "processor.h"
class ProcessorFactory {
    public:
        std::unique_ptr<Processor> createProcessor(int type, int time_delta);
};

#endif