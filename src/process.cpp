#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "linux_parser.h"
#include "process.h"
#include "process_data.h"

using std::string;
using std::to_string;
using std::vector;

Process::Process(ProcessData process_data){
    _process_data = process_data;
}

bool isValid() {
    return true;
}

int Process::Pid() { 
    return _process_data._pid;
}

float Process::CpuUtilization() const{
    float clock_tck = (float)(sysconf(_SC_CLK_TCK));
    long process_total_time__sec = _process_data._utime__sec +
                                   _process_data._stime__sec +
                                   _process_data._cutime__sec +
                                   _process_data._cstime__sec;
    float process_uptime__sec = 
        _process_data._sys_uptime__sec - _process_data._starttime__sec;
    return (100.0 * process_total_time__sec/process_uptime__sec);
}

string Process::Command() { 
    return _process_data._cmdline;
}

string Process::Ram() {
    return to_string(_process_data._VmSize__kB);
}

string Process::User() { 
    return _process_data._User;
 }

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() {
    return _process_data._sys_uptime__sec - _process_data._starttime__sec;
 }

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a) const { 
    if ( CpuUtilization() < a.CpuUtilization() ) {
        return true;
    }
    return false;
}