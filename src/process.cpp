#include "process.h"

bool Process::comparePtrToProcesses(const Process* const a, const Process* const b) {
    return (a->CpuUtilization() < b->CpuUtilization());
}

bool Process::operator<(Process const& a) const { 
    return ( CpuUtilization() < a.CpuUtilization() );
}