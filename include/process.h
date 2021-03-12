#ifndef PROCESS_H
#define PROCESS_H

#include <string>

class Process {
  public:
    virtual int Pid() const = 0;                               
    virtual std::string User() const = 0;                  
    virtual std::string Command() const = 0;             
    virtual float CpuUtilization() const = 0;          
    virtual std::string Ram__MB() const = 0;                       
    virtual long int UpTime() const = 0;                
    virtual bool isValid() const = 0;
    bool operator<(Process const& a) const;
    static bool comparePtrToProcesses(const Process* const a, const Process* const b);
};

#endif