#include "system_factory.h"
#include "basic_system.h"
#include "processor_factory.h"


std::unique_ptr<System> SystemFactory::createSystem( 
                                        int system_type, 
                                        int processor_type, 
                                        int processor_delta_time, 
                                        int process_type) {
    ProcessorFactory processor_factory = ProcessorFactory();
    if (system_type > 0 || system_type <= 0 || process_type > 0) {
    // Only one type of System exists, so return that type, BasicSystem.
        std::unique_ptr<Processor> p_processor = 
            processor_factory.createProcessor(processor_type, processor_delta_time);
        std::unique_ptr<BasicSystem> basic_system = 
            std::make_unique<BasicSystem>(std::move(p_processor), process_type);
        basic_system->updateSystem();
        return std::move(basic_system);
    }
    return nullptr;
}
