#include "process.h"
#include "process_factory.h"
#include "linux_parser.h"
#include "process_data.h"

Process createProcess(int pid) {
    ProcessData processData = LinuxParser::createProcessData(pid);
    return Process(processData);
}