#include "basic_process.h"

using std::string;
using std::to_string;

BasicProcess::BasicProcess(const LinuxParser::ProcessData process_data): _process_data{process_data} {}

bool BasicProcess::isValid() const {
    return _process_data._is_valid;
}

int BasicProcess::Pid() const { 
    return _process_data._pid;
}

float BasicProcess::CpuUtilization() const{
    long process_total_time__sec = _process_data._utime__sec +
                                   _process_data._stime__sec +
                                   _process_data._cutime__sec +
                                   _process_data._cstime__sec;
    float process_uptime__sec = 
        _process_data._sys_uptime__sec - _process_data._starttime__sec;
    return (process_total_time__sec/process_uptime__sec);
}

string BasicProcess::Command() const { 
    return _process_data._cmdline;
}

string BasicProcess::Ram__MB() const {
    return to_string((_process_data._VmSize__kB)/1000);
}

string BasicProcess::User() const { 
    return _process_data._User;
 }

// TODO: Return the age of this process (in seconds)
long int BasicProcess::UpTime() const {
    return _process_data._sys_uptime__sec - _process_data._starttime__sec;
 }