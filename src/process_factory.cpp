#include "process.h"
#include "process_factory.h"
#include "linux_parser.h"
#include "basic_process.h"

std::unique_ptr<Process> ProcessFactory::createProcess(int pid, int type) {
    LinuxParser::ProcessData processData = LinuxParser::createProcessData(pid);
    // only one type, so return BasicProcess type
    if (type <=0 || type > 0)
        return std::make_unique<BasicProcess>(processData);
    return nullptr;
}