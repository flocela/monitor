#include "system_factory.h"
#include "system.h"

System SystemFactory::createSystem(int system_type, int processor_type, int process_type) {
    if (system_type == 1) {
        return System(1, 1);
    }
    else 
        return System(1, 1);
}