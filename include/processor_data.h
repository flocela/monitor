#ifndef PROCESSOR_DATA_H
#define PROCESSOR_DATA_H

// How units are displayed examples: _utime__ct is user mode time in clock ticks.  
// _utime__sec is usermode time in seconds. xxx__kB is kiloByte ...

struct ProcessorData {
    long _user__ct     = -1; // from proc/stat/line 1 and field = 1
    long _nice__ct     = -1; // from proc/stat/line 1 and field = 2
    long _system__ct   = -1; // from proc/stat/line 1 and field = 3
    long _idle__ct     = -1; // from proc/stat/line 1 and field = 4
    long _iowait__ct   = -1; // from proc/stat/line 1 and field = 5
    long _irq__ct      = -1; // from proc/stat/line 1 and field = 6
    long _softirq__ct  = -1; // from proc/stat/line 1 and field = 7
    long _steal__ct    = -1; // from proc/stat/line 1 and field = 8
    int _procs_running = -1; // from proc/stat/ field procs_running
};

#endif