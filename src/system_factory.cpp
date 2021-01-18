#include "system_factory.h"
#include "basic_system.h"
#include "processor_factory.h"


System SystemFactory::createSystem( int system_type, 
                                    int processor_type, 
                                    int processor_delta_time, 
                                    int process_type) {
    ProcessorFactory processor_factory = ProcessorFactory();
    Processor processor = processor_factory.createProcessor(processor_type, processor_delta_time);
    // different type would create different Systems. Only one type exists right now, so return BasicSystem.
    System basic_system = BasicSystem(processor, 1);
    basic_system.updateSystem();
    return BasicSystem(processor, 1);
}
