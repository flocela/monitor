#include "ncurses_display.h"
#include "system.h"

#include "linux_parser.h"
#include <iostream>

#include <vector>
#include <string>

int main() {
  //System system;
  //NCursesDisplay::Display(system);
  std::cout << "Kernel(): 4.15.0-1080-gcp" << "\n";
  std::cout << "Kernel(): " << LinuxParser::Kernel() << "\n";
  std::cout << "Pids(): 1, 6, 7, 8, 29, 40, 41, 51, 52, 56, 63, 67, 76, 78, 85, 427, 434, 435, 440, 442, 453, 495, 1636, 1642, 1683, 1691, 1695, 1706, 1715, 1726, 1728, 1749, 1767, 1813, 1856, 1873, 1885, 1932, 2089, 2165, 2427, 2451, 2506, 2519, 2554" << "\n";
  std::cout << "Pids(): ";
  std::vector<int> pids = LinuxParser::Pids();
  for (std::size_t ii=0; ii<pids.size(); ii++) {
    if (ii < pids.size() - 1)
      std::cout << pids[ii] << ", ";
    else 
      std::cout << pids[ii] << "\n";
  }
  std::cout << "MemoryUtilization(): 1708096.000000" <<"\n";
  std::cout << "MemoryUtilization(): " << std::to_string(LinuxParser::MemoryUtilization()) << "\n";
  std::cout << "UpTime(): 2492" <<"\n";
  std::cout << "UpTime(): " << std::to_string(LinuxParser::UpTime()) << "\n";
  std::cout << "Jiffies(): 18331" <<"\n";
  std::cout << "Jiffies(): " << std::to_string(LinuxParser::Jiffies()) << "\n";
  std::cout << "ActiveJiffies(1): 493" <<"\n";
  std::cout << "ActiveJiffies(1): " << std::to_string(LinuxParser::ActiveJiffies(1)) << "\n";
  std::cout << "ActiveJiffies(): 76666" <<"\n";
  std::cout << "ActiveJiffies(): " << std::to_string(LinuxParser::ActiveJiffies()) << "\n";
  std::cout << "IdleJiffies(): 420015" <<"\n";
  std::cout << "IdleJiffies(): " << std::to_string(LinuxParser::IdleJiffies()) << "\n";
  std::cout << "CpuUtilization(): 51934, 6401, 18331, 417725, 2290, 0, 573, 0, 0, 0" <<"\n";
  std::cout << "CpuUtilization(): ";
  std::vector<std::string> cpuUtils = LinuxParser::CpuUtilization();
  for (std::size_t ii=0; ii<cpuUtils.size(); ii++) {
    if (ii < cpuUtils.size() - 1)
      std::cout << cpuUtils[ii] << ", ";
    else 
      std::cout << cpuUtils[ii] << "\n";
  }
  std::cout << "TotalProcesses(): 25229" <<"\n";
  std::cout << "TotalProcesses(): " << std::to_string(LinuxParser::TotalProcesses()) << "\n";
  std::cout << "RunningProcesses(): 2" <<"\n";
  std::cout << "RunningProcesses(): " << std::to_string(LinuxParser::RunningProcesses()) << "\n";
  std::cout << "Command(1): /bin/sh-c/usr/local/bin/start_desktop.sh" <<"\n";
  std::cout << "Command(1): " << LinuxParser::Command(1) << "\n";
  std::cout << "UpTime: 1699" <<"\n";
  std::cout << "UpTime: " << std::to_string(LinuxParser::UpTime(6)) << "\n";
}

