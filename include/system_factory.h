#ifndef SYSTEM_FACTORY_H
#define SYSTEM_FACTORY_H

#include <memory>
#include "linux_parser.h"
#include "system.h"

class SystemFactory {
    public:
        std::unique_ptr<System> createSystem(int system_type, int processor_type, int processor_delta_time, int process_type);
        void updateSystemData(LinuxParser::SystemData& system_data);
        
};

#endif