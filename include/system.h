#ifndef SYSTEM_H
#define SYSTEM_H

#include "processor.h"
#include "process.h"

#include <string>
#include <vector>
#include <memory>

class System {
  public:       
    virtual void updateSystem() = 0;                   
    virtual Processor const * Cpu() const = 0;                
    virtual std::vector<Process const *> Processes() const= 0;
    virtual float MemoryUtilization() const = 0;      
    virtual long UpTime() const = 0;                    
    virtual int TotalProcesses() const = 0;              
    virtual int RunningProcesses() const = 0;           
    virtual std::string Kernel() const = 0;              
    virtual std::string OperatingSystem() const = 0;
};

#endif