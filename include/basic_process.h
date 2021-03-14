#ifndef BASIC_PROCESS_H
#define BASIC_PROCESS_H

#include "process.h"
#include "linux_parser.h"
#include <string>

class BasicProcess: public Process {
  public:
    BasicProcess(const LinuxParser::ProcessData process_data);
    int Pid() const;                               
    std::string User() const;                      
    std::string Command() const;                   
    float CpuUtilization() const;
    // Returns VmData from proc/[pid]/stat                
    std::string Ram__MB() const;
    bool isValid() const;                       
    long int UpTime() const;
  
  private:
    LinuxParser::ProcessData _process_data;
};

#endif