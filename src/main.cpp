#include "ncurses_display.h"
#include "system.h"

#include "linux_parser.h"
#include <iostream>

#include <vector>
#include <string>

int main() {
  //System system;
  //NCursesDisplay::Display(system);
  std::cout << "Kernel(): " << LinuxParser::Kernel() << "\n";
  std::cout << "Pids(): ";
  std::vector<int> pids = LinuxParser::Pids();
  for (std::size_t ii=0; ii<pids.size(); ii++) {
    if (ii < pids.size() - 1)
      std::cout << pids[ii] << ", ";
    else 
      std::cout << pids[ii] << "\n";
  }
  std::cout << "MemoryUtilization(): " << std::to_string(LinuxParser::MemoryUtilization()) << "\n";
  std::cout << "UpTime(): " << std::to_string(LinuxParser::UpTime()) << "\n";
  std::cout << "Jiffies(): " << std::to_string(LinuxParser::Jiffies()) << "\n";
  std::cout << "ActiveJiffies(1): " << std::to_string(LinuxParser::ActiveJiffies(1)) << "\n";
  std::cout << "ActiveJiffies(): " << std::to_string(LinuxParser::ActiveJiffies()) << "\n";
  std::cout << "IdleJiffies(): " << std::to_string(LinuxParser::IdleJiffies()) << "\n";
  std::cout << "CpuUtilization(): ";
  std::vector<std::string> cpuUtils = LinuxParser::CpuUtilization();
  for (std::size_t ii=0; ii<cpuUtils.size(); ii++) {
    if (ii < cpuUtils.size() - 1)
      std::cout << cpuUtils[ii] << ", ";
    else 
      std::cout << cpuUtils[ii] << "\n";
  }
  std::cout << "TotalProcesses(): " << std::to_string(LinuxParser::TotalProcesses()) << "\n";
  std::cout << "RunningProcesses(): " << std::to_string(LinuxParser::RunningProcesses()) << "\n";
  std::cout << "Command(1): " << LinuxParser::Command(1) << "\n";
  std::cout << "Ram(1): " << LinuxParser::Ram(1) << "\n";
  std::cout << "Uid(1): " << LinuxParser::Uid(1) << "\n";
  std::cout << "User(1): " << LinuxParser::User(1) << "\n";
  std::cout << "UpTime: " << std::to_string(LinuxParser::UpTime(6)) << "\n";
}

