#ifndef SYSTEM_FACTORY_H
#define SYSTEM_FACTORY_H

class SystemFactory {
    public:
    System createSystem(int system_type, int processor_type, int process_type);


};

#endif