#ifndef SYSTEM_H
#define SYSTEM_H

#include "process_factory.h"
#include "processor.h"
#include "process.h"
#include "system_data.h"

#include <string>
#include <vector>

class System {
  public:
    System(int processor_type, int process_type);                           
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