#ifndef SYSTEM_H
#define SYSTEM_H

#include "process_factory.h"
#include "processor.h"
#include "process.h"
#include "system_data.h"

#include <string>
#include <vector>

class System {
 public:
  System();                           // Constructor
  Processor& Cpu();                   // TODO: See src/system.cpp
  std::vector<Process>& Processes();  // TODO: See src/system.cpp
  float MemoryUtilization();          // TODO: See src/system.cpp
  long UpTime();                      // TODO: See src/system.cpp
  int TotalProcesses();               // TODO: See src/system.cpp
  int RunningProcesses();             // TODO: See src/system.cpp
  std::string Kernel();               // TODO: See src/system.cpp
  std::string OperatingSystem();      // TODO: See src/system.cpp
  void updateSystem();
  // TODO: Define any necessary private members
 private:
  SystemData _system_data; 
  Processor _cpu;
  std::vector<Process> _processes = {};
  ProcessFactory _process_factory = ProcessFactory();
  void populateProcesses();
};

#endif