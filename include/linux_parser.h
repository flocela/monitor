#ifndef SYSTEM_PARSER_H
#define SYSTEM_PARSER_H

#include <string>
#include <vector>

namespace LinuxParser {
// Paths
const std::string kProcDirectory{"/proc/"};
const std::string kCmdlineFilename{"/cmdline"};
const std::string kCpuinfoFilename{"/cpuinfo"};
const std::string kStatusFilename{"/status"};
const std::string kStatFilename{"/stat"};
const std::string kUptimeFilename{"/uptime"};
const std::string kMeminfoFilename{"/meminfo"};
const std::string kVersionFilename{"/version"};
const std::string kOSPath{"/etc/os-release"};
const std::string kPasswordPath{"/etc/passwd"};

struct SystemData {
  std::string _os_name        = ""; // /ect/os-release
  std::string _kernel_version = ""; // /proc/version
  std::vector<int> _pids      = {};
  float _mem_total__kB        = -1; // /proc/meminfo
  float _mem_free__kB         = -1; // /proc/meminfo
  long  _up_time__sec         = -1; // /proc/uptime
  int  _procs_running         = -1; // from proc/stat/ field procs_running
};

struct ProcessorData {
  long  _user__ct             = -1; // from proc/stat/line 1 and field = 1
  long  _nice__ct             = -1; // from proc/stat/line 1 and field = 2
  long  _system__ct           = -1; // from proc/stat/line 1 and field = 3
  long  _idle__ct             = -1; // from proc/stat/line 1 and field = 4
  long  _iowait__ct           = -1; // from proc/stat/line 1 and field = 5
  long  _irq__ct              = -1; // from proc/stat/line 1 and field = 6
  long  _softirq__ct          = -1; // from proc/stat/line 1 and field = 7
  long  _steal__ct            = -1; // from proc/stat/line 1 and field = 8
};

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

ProcessData   createProcessData(int pid);
SystemData    createSystemData();
ProcessorData createProcessorData();

}; 

#endif