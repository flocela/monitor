#include "process.h"
#include "processor.h"
#include "system.h"
#include "linux_parser.h"
#include "basic_system.h"

#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>
#include <iostream>

using std::set;
using std::size_t;
using std::string;
using std::vector;

BasicSystem::BasicSystem(int processor_type, int process_type): System{}, _cpu{processor_type} {
    _system_data = LinuxParser::createSystemData();
    populateProcesses();
}

// TODO: Return the system's CPU
Processor& BasicSystem::Cpu() { 
    return _cpu;
}

void BasicSystem::populateProcesses() {
    std::vector<Process> replacement = {};
    // if the process still exists, then create an object for it.
    // if not then it has been deleted since the vector of pids was made.
    for (int pid: _system_data._pids) {
        Process process = _process_factory.createProcess(pid);
        if (process.isValid())
            replacement.push_back(process);
    }
    _processes = replacement;
}

// Only returns valid processes.
vector<Process>& BasicSystem::Processes() {
    return _processes;
}

// TODO: Return the system's kernel identifier (string)
std::string BasicSystem::Kernel() { 
    return LinuxParser::Kernel(); 
}

// TODO: Return the system's memory utilization
float BasicSystem::MemoryUtilization() { 
    return LinuxParser::MemoryUtilization(); 
}

// TODO: Return the operating system name
std::string BasicSystem::::OperatingSystem() { 
    return LinuxParser::OperatingSystem(); 
}

// TODO: Return the number of processes actively running on the system
int BasicSystem::::RunningProcesses() { 
    return LinuxParser::RunningProcesses(); 
}

// TODO: Return the total number of processes on the system
int BasicSystem::::TotalProcesses() { 
    return LinuxParser::TotalProcesses();
}

// TODO: Return the number of seconds since the system started running
long int BasicSystem::::UpTime() { 
    return LinuxParser::UpTime(); 
}