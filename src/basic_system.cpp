#include "basic_system.h"

// public
BasicSystem::BasicSystem(std::unique_ptr<Processor> processor, int process_type):
                            System{}, 
                            _cpu{std::move(processor)},
                            _process_type { process_type}
{
    updateSystem();
}

void BasicSystem::updateSystem() {
    _sys_d = LinuxParser::createSystemData();
    _cpu->update();
    populateProcesses();
}

Processor const * BasicSystem::Cpu() const{ 
    return _cpu.get();
}

vector<Process const *> BasicSystem::Processes() const {
    vector<Process const *> return_vector;
    for (auto& pr : _processes) {
        return_vector.push_back(pr.get());
    }
    return return_vector;
}

float BasicSystem::MemoryUtilization() const { 
    return (float)(_sys_d._mem_total__kB - _sys_d._mem_free__kB) / (float)(_sys_d._mem_total__kB);
}

long int BasicSystem::UpTime() const { 
    return _sys_d._up_time__sec;
}

int BasicSystem::TotalProcesses() const { 
    return _processes.size();
}

int BasicSystem::RunningProcesses() const { 
    return _sys_d._procs_running; 
}

string BasicSystem::Kernel() const { 
    return _sys_d._kernel_version;
}

string BasicSystem::OperatingSystem() const {
    return _sys_d._os_name;
}

void BasicSystem::populateProcesses() {
    vector<std::unique_ptr<Process>> replacement = {};
    // if the process still exists, then create an object for it.
    // if not then it has been deleted since the vector of pids was made
    for (int pid: _sys_d._pids) {
        std::unique_ptr<Process> process = 
            _process_factory.createProcess(pid, _process_type);
        if (process->isValid()) 
            replacement.push_back(std::move(process));
    }
    _processes = std::move(replacement);
}