#include "process.h"
#include "processor.h"
#include "system.h"
#include "linux_parser.h"

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

// TODO: Return the system's CPU
Processor& System::Cpu() { return _cpu; }

// Only returns valid processes.
vector<Process>& System::Processes() {
    std::vector<Process> replacement = {};
    vector<int> pids = LinuxParser::Pids();
    for (int pid: pids) {
        Process process = _process_factory.createProcess(pid);
        if (process.isValid())
            replacement.push_back(process);
    }
    _processes = replacement;
    return _processes;
}

// TODO: Return the system's kernel identifier (string)
std::string System::Kernel() { 
    return LinuxParser::Kernel(); 
}

// TODO: Return the system's memory utilization
float System::MemoryUtilization() { 
    return LinuxParser::MemoryUtilization(); 
}

// TODO: Return the operating system name
std::string System::OperatingSystem() { 
    return LinuxParser::OperatingSystem(); 
}

// TODO: Return the number of processes actively running on the system
int System::RunningProcesses() { 
    return LinuxParser::RunningProcesses(); 
}

// TODO: Return the total number of processes on the system
int System::TotalProcesses() { 
    return LinuxParser::TotalProcesses();
}

// TODO: Return the number of seconds since the system started running
long int System::UpTime() { 
    return LinuxParser::UpTime(); 
}