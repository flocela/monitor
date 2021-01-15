#ifndef SYSTEM_DATA_H
#define SYSTEM_DATA_H

#include <string>
#include <vector>

// How units are displayed examples: _utime__ct is user mode time in clock ticks.  
// _utime__sec is usermode time in seconds. xxx__kB is kiloByte ...

struct SystemData {
    std::string _os_name        = ""; // /ect/os-release
    std::string _kernel_version = ""; // /proc/version
    std::vector<int> _pids      = {};
    int _num_of_pids            = -1;
    float _mem_total__kB        = -1; // /proc/meminfo
    float _mem_free__kB         = -1; // /proc/meminfo
    long _up_time__sec          = -1; // /proc/uptime
    long _user__ct              = -1; // /proc/stat/(line = cpu, field = 1)
    long _nice__ct              = -1; // /proc/stat/(line = cpu, field = 2)
    long _system__ct            = -1; // /proc/stat/(line = cpu, field = 3)
    long _idle__ct              = -1; // /proc/stat/(line = cpu, field = 4)
    long _iowait__ct            = -1; // /proc/stat/(line = cpu, field = 5)
    long _irq__ct               = -1; // /proc/stat/(line = cpu, field = 6)
    long _soft_irq__ct          = -1; // /proc/stat/(line = cpu, field = 7)
    long _steal__ct             = -1; // /proc/stat/(line = cpu, field = 8)
    int _procs_running          = -1; // /proc/stat/(field = procs_running)
    float _cpu_util             = -1.0;
};

#endif