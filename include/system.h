#ifndef SYSTEM_H
#define SYSTEM_H

#include "processor.h"
#include "process.h"

#include <string>
#include <vector>

class System {
  public:                          
    virtual Processor& Cpu();                   
    virtual std::vector<Process>& Processes();  
    virtual float MemoryUtilization();          
    virtual long UpTime();                      
    virtual int TotalProcesses();               
    virtual int RunningProcesses();             
    virtual std::string Kernel();               
    virtual std::string OperatingSystem();      
    virtual void updateSystem();
};

#endif