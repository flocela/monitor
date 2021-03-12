#include "processor_factory.h"
#include "basic_processor.h"

std::unique_ptr<Processor> ProcessorFactory::createProcessor(int processor_type, int time_delta) {
    // Only one type of Processor, the BasicProcessor.
    return std::make_unique<BasicProcessor>(time_delta);
}