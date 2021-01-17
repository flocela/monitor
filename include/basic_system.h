#ifndef BASIC_SYSTEM_H
#define BASIC_SYSTEM_H

#include "process_factory.h"
#include "processor.h"
#include "process.h"
#include "system_data.h"
#include "system.h"

#include <string>
#include <vector>

class BasicSystem: virtual public System {
  public:
    BasicSystem(int processor_type, int process_type);                           
    Processor& Cpu();                   
    std::vector<Process>& Processes();  
    float MemoryUtilization();          
    long UpTime();                      
    int TotalProcesses();               
    int RunningProcesses();             
    std::string Kernel();               
    std::string OperatingSystem();      
    void updateSystem();
  
  private:
    SystemData _system_data; 
    Processor _cpu;
    std::vector<Process> _processes = {};
    ProcessFactory _process_factory = ProcessFactory();
    void populateProcesses();
};

#endif