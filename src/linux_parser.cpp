#include "process_data.h"
#include "linux_parser.h"
#include "processor.h"

#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <regex>
#include <iostream>

using std::stof;
using std::string;
using std::to_string;
using std::vector;
using std::regex;
using std::stoi;

namespace LinuxParser {
  std::string::size_type sz;

  // update process_Data with sys_uptime
  void updateProcessData_SysUptime (ProcessData process_data);

  // update process_data with data from ".../pid/kStatFilename"
  void updateProcessData_kStatFilename (string pid_str, ProcessData process_data);

  // update process_data with ".../pid/cmdline"
  void updateProcessData_cmdline (string pid_str, ProcessData process_data);
  
  // update process_data with Uid and User name
  void updateProcessData_UidUser (string pid_str, ProcessData process_data);

  // update process_data with Vm data 
  void updateProcessData_Vm (int pid, ProcessData process_data);

  // update system_data with os_name from "/ect/os-release"
  void updateSystemData_os_name(SystemData system_data);

  // udate system_data with kernel_version from "/version"
  void updateSystemData_kernel_version(SystemData system_data);

  // update system_data with processor id's
  void updateSystem_PIDS(SystemData system_data);

  // update system_data with mem info from "proc/meminfo"
  void updateSystem_mem_info(SystemData system_data);

  // update system_data upTime from "/proc/uptime"
  void updateSystemData_up_time(SystemData system_data);

  // update system_data with data from "proc/stat"
  void updateSystemData_kStatFilename(SystemData system_data);

  // update processor_data with data from "/proc/stat"
  void updateProcessorData_kStatFilename(ProcessorData processor_data);

  string getUserNameFromUid(string Uid);
  // Returns everything before the colon in the given str.
  // Example: If str is "MemTotal:    1234kB", will return "MemTotal".
  string getStringBeforeColon(string str);

  // Returns int value before "kB" in the given str.
  // Example: "MemTotal:    1234 kB" will return 1234.
  int getkB(string str);

  // Splits a str into words. Words are separated as per isstringstream (words are 
  //  separated by spaces).
  vector<std::string> split(string str);

  // Opens a file described by @path. Returns the first line in the file
  // that begins with @word.
  string getLineStartingWith(string word, string path);

  // Splits @str into words as described in isstring (words are separated by spaces).
  // Returns the word at @position. @position is zero-indexed.
  string getWordAt (string text, int position);

  // Opens a file described by @path.  Returns the first line int the file.
  string getLineFromPath(string path);

  // Splits @line into words as described in isstring (words are separated by spaces).
  // Assumes all the words are designated in @v by their position are longs.
  // Adds all the longs at the positions enumerated by @v and returns the sum.
  long getLongSumFromWordsAt(string line, vector<int> v);

  // Returns sum same as getLongSumFromWordsAt(..), but returns an int.
  int getIntSumFromWordsAt(string line, vector<int> v);
}

ProcessData LinuxParser::createProcessData(int pid) {
  string pid_str = std::to_string(pid);
  ProcessData process_data = ProcessData();
  
  // TODO: These all open different files. Wish I could
  // synchronize all the file openings and get all the information
  // at once. Not really possible??. At at least they're all in
  // one method. Here.
  process_data._pid = pid;
  updateProcessData_cmdline(pid_str, process_data);
  updateProcessData_kStatFilename(pid_str, process_data);
  updateProcessData_UidUser(pid_str, process_data);
  updateProcessData_Vm(pid, process_data);

  if ( process_data._cmdline.empty()  ||
       process_data._utime__ct == -1  ||
       process_data._Uid.empty()      ||
       process_data._User.empty()     ||
       process_data._VmSize__kB == -1   ) {
         process_data._is_valid = false;
  }

  return process_data;
}

void LinuxParser::updateProcessData_SysUptime (ProcessData process_data) {
  process_data._sys_uptime__sec = UpTime();
}

void LinuxParser::updateProcessData_cmdline (string pid_str, ProcessData process_data) {
  process_data._cmdline = 
    getLineFromPath(kProcDirectory + 
                    pid_str +
                    kCmdlineFilename);
}

void LinuxParser::updateProcessData_kStatFilename (string pid_str, ProcessData process_data) {
  string stat_line = 
    getLineFromPath(kProcDirectory + 
                    pid_str +
                    kStatFilename);
  if (stat_line.empty())
   return;
  vector<string> items = split(stat_line);
  process_data._pid            = stoi(items[0], &sz);
  process_data._utime__ct      = stol(items[13], nullptr, 10);
  process_data._stime__ct      = stol(items[14],  nullptr, 10);
  process_data._cutime__ct     = stol(items[15], nullptr, 10);
  process_data._cstime__ct     = stol(items[16], nullptr, 10);
  process_data._starttime__ct  = stol(items[21], nullptr, 10);
  process_data._utime__sec     = process_data._utime__ct/sysconf(_SC_CLK_TCK);
  process_data._stime__sec     = process_data._stime__ct/sysconf(_SC_CLK_TCK);
  process_data._cutime__sec    = process_data._cutime__ct/sysconf(_SC_CLK_TCK);
  process_data._cstime__sec    = process_data._cstime__ct/sysconf(_SC_CLK_TCK);
  process_data._starttime__sec = process_data._starttime__ct/sysconf(_SC_CLK_TCK);
}

void LinuxParser::updateProcessData_UidUser (string pid_str, ProcessData process_data) {
  string uid_line =
    getLineStartingWith("Uid:",
                        kProcDirectory + 
                        pid_str + 
                        kStatusFilename);
  if (!uid_line.empty()) {
    vector<string> items = split(uid_line);
    process_data._Uid = items[1];
    process_data._User = getUserNameFromUid(process_data._Uid);
  }
  
}

string LinuxParser:: getUserNameFromUid (string Uid) {
  string lookfor = "x:" + Uid + ":" + Uid + ":";
  string line;
  std::ifstream filestream(kPasswordPath);
  if (filestream.is_open()) {
    while(std::getline(filestream, line)) {
      if (line.find(lookfor) != std::string::npos) {
        return LinuxParser::getStringBeforeColon(line);
      }
    }
  }
  return "";
}

void LinuxParser::updateProcessData_Vm(int pid, ProcessData process_data) {
  string line;
  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  if (filestream.is_open()) {
    while(std::getline(filestream, line)) {
      string name = LinuxParser::getStringBeforeColon(line);
      if (name == "VmSize")
        process_data._VmSize__kB = LinuxParser::getkB(line);
    }
  }
}

LinuxParser::SystemData LinuxParser::createSystemData() {
  SystemData system_data = SystemData();
  
}

void LinuxParser::updateSystemData_os_name(SystemData system_data) {
  string pretty_line = getLineStartingWith("PRETTY_NAME", kOSPath);
  system_data._os_name = pretty_line.substr(14, pretty_line.size()-1);
}

void LinuxParser::updateSystemData_kernel_version(SystemData system_data) {
  string os, version, kernel;
  string line = LinuxParser::getLineFromPath(kProcDirectory + kVersionFilename);
  system_data._kernel_version = getWordAt(line, 2);
}

void LinuxParser::updateSystem_PIDS(SystemData system_data) {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  system_data._pids = pids;
}

void LinuxParser::updateSystem_mem_info(SystemData system_data) {
  string line;
  std::ifstream filestream(kProcDirectory + kMeminfoFilename);
  if(filestream.is_open()) {
    while(std::getline(filestream, line)) {
      string name = LinuxParser::getStringBeforeColon(line);
      if(name == "MemTotal")
        system_data._mem_total__kB = LinuxParser::getkB(line);
      else if(name == "MemFree")
        system_data._mem_free__kB = LinuxParser::getkB(line);
    }
  }
}

void LinuxParser::updateSystemData_up_time(SystemData system_data) {
  string line = LinuxParser::getLineFromPath(kProcDirectory + kUptimeFilename);
  system_data._up_time__sec = LinuxParser::getLongSumFromWordsAt(line, {0});
}

void LinuxParser::updateSystemData_kStatFilename(SystemData system_data) {
  string stat_line = "";
  string procs_running_line = "";
  string first = "";
  string line = "";
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> first;
      if (first == "cpu")
        stat_line = line;
      else if (first == "procs_running")
        procs_running_line = line;
    }
  }
  vector<string> items = split(stat_line);
  system_data._user__ct    = stol(items[1], nullptr, 10);
  system_data._nice__ct    = stol(items[2], nullptr, 10);
  system_data._system__ct  = stol(items[3], nullptr, 10);
  system_data._idle__ct    = stol(items[4], nullptr, 10);
  system_data._iowait__ct  = stol(items[5], nullptr, 10);
  system_data._irq__ct     = stol(items[6], nullptr, 10);
  system_data._softirq__ct = stol(items[7], nullptr, 10);
  system_data._steal__ct   = stol(items[8], nullptr, 10);

  system_data._procs_running = stoi(getWordAt(procs_running_line, 1), &sz);
}

ProcessorData LinuxParser::createProcessorData() {
  ProcessorData processor_data = ProcessorData();
  updateProcessorData_kStatFilename(processor_data);
}

void LinuxParser::updateProcessorData_kStatFilename(ProcessorData processor_data) {
  string stat_line = "";
  string procs_running_line = "";
  string first = "";
  string line = "";
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> first;
      if (first == "cpu")
        stat_line = line;
      else if (first == "procs_running")
        procs_running_line = line;
    }
  }
  vector<string> items = split(stat_line);
  processor_data._user__ct    = stol(items[1], nullptr, 10);
  processor_data._nice__ct    = stol(items[2], nullptr, 10);
  processor_data._system__ct  = stol(items[3], nullptr, 10);
  processor_data._idle__ct    = stol(items[4], nullptr, 10);
  processor_data._iowait__ct  = stol(items[5], nullptr, 10);
  processor_data._irq__ct     = stol(items[6], nullptr, 10);
  processor_data._softirq__ct = stol(items[7], nullptr, 10);
  processor_data._steal__ct   = stol(items[8], nullptr, 10);

  processor_data._procs_running = stoi(getWordAt(procs_running_line, 1), &sz);
}
// DONE: An example of how to read data from the filesystem
// !!!!!!!!!!!!!!!!!!!!!!!!!! DONE !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}


// TODO Probably get rid of this.
// BONUS: Update this to use std::filesystem
// !!!!!!!!!!!!!!!!!!!!!!!!!! DONE !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// TODO: Read and return the number of jiffies for the system
// !!!!!!!!!!!!!!!!!!!!!!!!!! DONE !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
long LinuxParser::Jiffies()  { 
  string line = LinuxParser::getLineFromPath(kProcDirectory + kStatFilename);
  return LinuxParser::getLongSumFromWordsAt(line, {3});
}

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
// !!!!!!!!!!!!!!!!!!!!!!!!!! DONE !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
long LinuxParser::ActiveJiffies(int pid) { 
  string line = LinuxParser::getLineFromPath(kProcDirectory + to_string(pid) + kStatFilename);
  return LinuxParser::getLongSumFromWordsAt(line, {13, 14, 15, 16});
}

// TODO: Read and return the number of active jiffies for the system
// !!!!!!!!!!!!!!!!!!!!!!!!!! DONE !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
long LinuxParser::ActiveJiffies() { 
  string line = LinuxParser::getLineFromPath(kProcDirectory + kStatFilename);
  return LinuxParser::getLongSumFromWordsAt(line, {1, 2, 3});
}

// TODO: Read and return the number of idle jiffies for the system
// !!!!!!!!!!!!!!!!!!!!!!!!!! DONE !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
long LinuxParser::IdleJiffies() { 
  string line = LinuxParser::getLineFromPath(kProcDirectory + kStatFilename);
  return LinuxParser::getLongSumFromWordsAt(line, {4, 5});
}

// TODO: Read and return CPU utilization line. cpu user nice system ... guest guest_nice.
vector<string> LinuxParser::CpuUtilization() { 
  vector<string> result = {};
  string line = LinuxParser::getLineFromPath(kProcDirectory + kStatFilename);
  if(line != "") {
    result = LinuxParser::split(line);
  }
  return result; 
}

// !!!!!!!!!!!!!!!!!! DONE !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
float LinuxParser::CpuUtilization(int pid) {
  long sys_uptime = LinuxParser::UpTime();
  float clock_tck = (float)(sysconf(_SC_CLK_TCK));
  string line = LinuxParser::getLineFromPath(LinuxParser::kProcDirectory + 
                                             std::to_string(pid) + 
                                             LinuxParser::kStatFilename);
  if (!line.empty()) {
    int process_id = LinuxParser::getLongSumFromWordsAt(line, {0});
    std::cout << "process_id test at zero: " << process_id << "\n";

    long total_time =  LinuxParser::getLongSumFromWordsAt(line, {14, 15, 16, 17}) / clock_tck;
    long process_start_time = LinuxParser::getLongSumFromWordsAt(line, {22}) / clock_tck;
    long process_uptime = sys_uptime - process_start_time;
    std::cout << "\n";
    std::cout << "_SC_CLK_TCK: " << sysconf(_SC_CLK_TCK) << "\n";
    std::cout << "sys_uptime: " << sys_uptime << "\n";
    std::cout << "total_time: " << total_time << "\n";
    std::cout << "start_time: " << process_start_time << "\n";
    std::cout << "process_uptime: " << process_uptime << "\n";

    return ( 100.0 * total_time / process_uptime );
  }
  else
    return -1;
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
//!!!!!!!!!!!!!!!!!!!!!! DONE !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
string LinuxParser::Command(int pid) {
  string path = kProcDirectory + std::to_string(pid) + kCmdlineFilename;
  return LinuxParser::getLineFromPath(path);
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
//!!!!!!!!!!!!!!!!!!!!!! DONE !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
string LinuxParser::Ram(int pid) { 
  string line;
  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  if (filestream.is_open()) {
    while(std::getline(filestream, line)) {
      string name = LinuxParser::getStringBeforeColon(line);
      if (name == "VmSize")
        return std::to_string(LinuxParser::getkB(line));
    }
  }
  return string(); 
}

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
//!!!!!!!!!!!!!!!!!!!!!! DONE !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
string LinuxParser::Uid(int pid) {
  string line = LinuxParser::getLineStartingWith("Uid:", kProcDirectory + std::to_string(pid) + 
  kStatusFilename);
  return LinuxParser::getWordAt(line, 1);
}

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
//!!!!!!!!!!!!!!!!!!!!!! DONE !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
string LinuxParser::User(int pid) {
  string uid = Uid(pid);
  string lookfor = "x:" + uid + ":" + uid + ":";
  string line;
  std::ifstream filestream(kPasswordPath);
  if (filestream.is_open()) {
    while(std::getline(filestream, line)) {
      if (line.find(lookfor) != std::string::npos) {
        return LinuxParser::getStringBeforeColon(line);
      }
    }
  }
  return string();
}

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
// In Seconds
long LinuxParser::UpTime(int pid) {
  string line = LinuxParser::getLineFromPath(kProcDirectory + to_string(pid) + kStatFilename);
  if (!line.empty()) {
    long pidStartTime = LinuxParser::getLongSumFromWordsAt(line, {21});
    long upTime = UpTime(); // This is in seconds
    return ( upTime - (pidStartTime / sysconf(_SC_CLK_TCK)) );
  }
  else
    return 0l;
}

string LinuxParser::getStringBeforeColon(string str) {
  int n = str.length();
  int j = 0;
  while (j < n) {
    if (str[j] == ':')
      break;
    j++;
  }
  return str.substr(0, j);
}

int LinuxParser::getkB(string str) {
  int n = str.length();
  int j = 0;
  int fn = 0;
  int ln = 0;
  while (j < n) {
    if (str[j] == ' ') {
      break;
    }
    j++;
  }
  while (j < n) {
    if (str[j] != ' ') {
      fn = j;
      break;
    }
    j++;
  }
  while (j < n) {
    if (str[j] == ' ') {
      ln = j - 1;
      break;
    }
    j++;
  }
  return stoi(str.substr(fn,ln - fn + 1), &sz);
}

vector<std::string> LinuxParser::split(string str) {
  std::vector<std::string> result;
  std::istringstream iss(str);
  for(std::string s; iss >> s;)
    result.push_back(s);
  return result;
}

string LinuxParser::getLineStartingWith(string word, string path) {
  std::ifstream filestream(path);
  string line;
  string first;
  if(filestream.is_open()) {
    while( std::getline(filestream, line) ) {
      std::istringstream linestream(line);
      linestream >> first;
      if (first == word){
        return line;
      }
    }
  }
  return "";
}

// First position is 0.
string LinuxParser::getWordAt (string text, int position) {
  if (text != "" && position >= 0) {
    vector<string> splitted = LinuxParser::split(text);
    if ( position < static_cast<int>(splitted.size()) )
      return splitted[position];
  }
  return "";
}

string LinuxParser::getLineFromPath(string path) {
  std::ifstream filestream(path);
  if ( filestream.is_open() ) {
    string line;
    std::getline(filestream, line);
    return line;
  }
  return "";
}

long LinuxParser::getLongSumFromWordsAt(string line, vector<int> v) {
  vector<string> items = split(line);
  long sum = 0l;
  for (std::size_t pos: v) {
    if (pos < items.size())
      sum +=  stol(items[pos], nullptr, 10);
    else
      return -1;
  }
  return sum;
}

int LinuxParser::getIntSumFromWordsAt(string line, vector<int> v) {
  vector<string> items = split(line);
  int sum = 0;
  for (std::size_t pos: v) {
    if (pos < items.size())
      sum +=  stoi(items[pos], &sz);
    else
      return -1;
  }
  return sum;
}
