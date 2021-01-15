#ifndef PROCESS_DATA_H
#define PROCESS_DATA_H

#include <string>

// How units are displayed examples: _utime__ct is user mode time in clock ticks.  
// _utime__sec is usermode time in seconds. xxx__kB is kiloByte ...

struct ProcessData {
    int  _sys_uptime__sec = -1; // /proc/uptime
    int  _pid             = -1; // /proc/pid/stat/(field = 1)
    long _utime__ct       = -1; // /proc/pid/stat/(field = 14)
    long _utime__sec      = -1;
    long _stime__ct       = -1; // /proc/pid/stat/(field = 15)
    long _stime__sec      = -1;
    long _cutime__ct      = -1; // /proc/pid/stat/(field = 16)
    long _cutime__sec     = -1;
    long _cstime__ct      = -1; // /proc/pid/stat/(field = 17)
    long _cstime__sec     = -1;
    long _starttime__ct   = -1; // /proc/pid/stat/(field = 22)
    long _starttime__sec  = -1;

    std::string _cmdline = ""; // /proc/pid/cmdline
    int _VmSize__kB      = -1; // /proc/pid/status
    std::string _Uid     = ""; // /proc/pid/status
    std::string _User    = ""; // /etc/password (matches Uid)

    bool _is_valid = false;
};

#endif