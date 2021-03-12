#ifndef BASIC_SYSTEM_H
#define BASIC_SYSTEM_H

#include "system.h"
#include "process_factory.h"
#include "processor.h"
#include "process.h"
#include "linux_parser.h"

using std::string;
using std::vector;

class BasicSystem: public System {
  public:
    BasicSystem(std::unique_ptr<Processor> processor, int process_type);                  
    
    void updateSystem() override;
    Processor const* Cpu() const;
    vector<Process const *> Processes() const;
    float      MemoryUtilization() const ;
    long       UpTime() const ; 
    int        TotalProcesses() const ;
    int        RunningProcesses() const ;
    string     Kernel() const ;                     
    string     OperatingSystem() const ;                       
    
  private:
    LinuxParser::SystemData _sys_d; 
    std::unique_ptr<Processor> _cpu;
    int _process_type;
    std::vector<std::unique_ptr<Process>> _processes = {};
    ProcessFactory _process_factory = ProcessFactory();
    void populateProcesses();
};

#endif