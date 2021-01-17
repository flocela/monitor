#include "processor_factory.h"
#include "basic_processor.h"

Processor ProcessorFactory::createProcessor(int processor_type, int time_delta) {
    if (processor_type == 1) {
        return BasicProcessor(time_delta);
    }
    else {
        return BasicProcessor(time_delta);
    }
}