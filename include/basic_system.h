#ifndef BASIC_SYSTEM_H
#define BASIC_SYSTEM_H

#include "process_factory.h"
#include "processor.h"
#include "process.h"
#include "system.h"

#include <string>
#include <vector>

class BasicSystem: virtual public System {
  public:
    BasicSystem(Processor processor, int process_type);
    void updateSystem();                           
    Processor& Cpu();                   
    std::vector<Process>& Processes();  
    float MemoryUtilization();          
    long UpTime();                      
    int TotalProcesses();               
    int RunningProcesses();             
    std::string Kernel();               
    std::string OperatingSystem();      
    
    int getSystemType();
    int getProcessorType();
    int getProcessesType();
  
  private:
    LinuxParser::SystemData _sys_d; 
    Processor _cpu;
    std::vector<Process> _processes = {};
    ProcessFactory _process_factory = ProcessFactory();
    void populateProcesses();
    int _system_type;
    int _processor_type;
    int _processes_type;
};

#endif