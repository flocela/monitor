#include "system_factory.h"
#include "basic_system.h"
#include "processor_factory.h"


System SystemFactory::createSystem( int system_type, 
                                    int processor_type, 
                                    int processor_delta_time, 
                                    int process_type) {
    ProcessorFactory processor_factory = ProcessorFactory();
    Processor processor = processor_factory.createProcessor(processor_type, processor_delta_time);
    if (system_type == 1) {
        return BasicSystem(1, 1);
    }
    else 
        return BasicSystem(1, 1);
}