#ifndef SYSTEM_FACTORY_H
#define SYSTEM_FACTORY_H

#include "system.h"

class SystemFactory {
    public:
        System createSystem(int system_type, int processor_type, int processor_delta_time, int process_type);
        void updateSystemData(SystemData system_data);
        
};

#endif