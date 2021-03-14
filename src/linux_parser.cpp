#include "linux_parser.h"

#include <dirent.h>
#include <unistd.h>
#include <regex>
#include <fstream>

using std::string;
using std::vector;
using std::regex;

namespace LinuxParser {
  string::size_type sz;

  // update process_Data with sys_uptime
  void updateProcessData_SysUptime (ProcessData& process_data);

  // update process_data with ".../pid/cmdline"
  void updateProcessData_cmdline (string pid_str, ProcessData& process_data);

  // update process_data with data from ".../pid/kStatFilename"
  void updateProcessData_kStatFilename (string pid_str, ProcessData& process_data);
  
  // update process_data with Uid and User name
  void updateProcessData_UidUser (string pid_str, ProcessData& process_data);

  // update process_data with Vm data 
  void updateProcessData_Vm (int pid, ProcessData& process_data);

  // update system_data with os_name from "/ect/os-release"
  void updateSystemData_os_name(SystemData& system_data);

  // udate system_data with kernel_version from "/version"
  void updateSystemData_kernel_version(SystemData& system_data);

  // update system_data with processor id's
  void updateSystem_PIDS(SystemData& system_data);

  // update system_data with mem info from "proc/meminfo"
  void updateSystem_mem_info(SystemData& system_data);

  // update system_data upTime from "/proc/uptime"
  void updateSystemData_up_time(SystemData& system_data);

  // update system_data with data from "proc/stat"
  void updateSystemData_kStatFilename(SystemData& system_data);

  string getUserNameFromUid(string Uid);

  long UpTime();

  // Returns everything before the colon in the given str.
  // Example: If str is "MemTotal:    1234 kB", will return "MemTotal".
  string getTextBeforeColon(string str);

  // Returns int value before "kB" in the given str.
  // Example: "MemTotal:    1234 kB" will return 1234.
  int getkB(string str);

  // Splits a str into words. Words are separated as per isstringstream (words are 
  //  separated by spaces).
  vector<string> split(string str);

  // Opens a file described by @path. Returns the first line in the file
  // that begins with @word.
  string getLineStartingWith(string word, string path);

  // Splits @str into words as described in isstring (words are separated by spaces).
  // Returns the word at @position. @position is zero-indexed.
  string getWordAt (string text, int position);

  // Opens a file described by @path.  Returns the first line in the file.
  string getFirstLineFromPath(string path);
}

LinuxParser::ProcessData LinuxParser::createProcessData(int pid) {
  string pid_str = std::to_string(pid);
  ProcessData process_data = ProcessData();

  process_data._pid = pid;
  updateProcessData_SysUptime(process_data);
  updateProcessData_cmdline(pid_str, process_data);
  updateProcessData_kStatFilename(pid_str, process_data);
  updateProcessData_UidUser(pid_str, process_data);
  updateProcessData_Vm(pid, process_data);

  process_data._is_valid = 
    ( ( process_data._cmdline.empty()  ||
        process_data._Uid.empty()      ||
        process_data._User.empty()     ||
        process_data._utime__ct == -1  ||
        process_data._VmSize__kB == -1   ) ?
      false : 
      true );

  return process_data;
}

LinuxParser::SystemData LinuxParser::createSystemData() {
  SystemData system_data = SystemData();
  updateSystemData_os_name(system_data);
  updateSystemData_kernel_version(system_data);
  updateSystem_PIDS(system_data);
  updateSystem_mem_info(system_data);
  updateSystemData_up_time(system_data);
  updateSystemData_kStatFilename(system_data);
  return system_data;
}

LinuxParser::ProcessorData LinuxParser::createProcessorData() {
  ProcessorData processor_data = ProcessorData();
  string stat_line = getLineStartingWith("cpu", kProcDirectory + kStatFilename);
  vector<string> items = split(stat_line);
  processor_data._user__ct    = stol(items[1], nullptr, 10);
  processor_data._nice__ct    = stol(items[2], nullptr, 10);
  processor_data._system__ct  = stol(items[3], nullptr, 10);
  processor_data._idle__ct    = stol(items[4], nullptr, 10);
  processor_data._iowait__ct  = stol(items[5], nullptr, 10);
  processor_data._irq__ct     = stol(items[6], nullptr, 10);
  processor_data._softirq__ct = stol(items[7], nullptr, 10);
  processor_data._steal__ct   = stol(items[8], nullptr, 10);
  return processor_data;
}

void LinuxParser::updateProcessData_SysUptime (ProcessData& process_data) {
  process_data._sys_uptime__sec = UpTime();
}

void LinuxParser::updateProcessData_cmdline (string pid_str, ProcessData& process_data) {
  process_data._cmdline = 
    getFirstLineFromPath(kProcDirectory + pid_str + kCmdlineFilename);
}

void LinuxParser::updateProcessData_kStatFilename (string pid_str, ProcessData& process_data) {
  string stat_line = 
    getFirstLineFromPath(kProcDirectory + pid_str + kStatFilename);
  if (!stat_line.empty()) {
    vector<string> items = split(stat_line);
    process_data._utime__ct      = stol(items[13], nullptr, 10);
    process_data._stime__ct      = stol(items[14], nullptr, 10);
    process_data._cutime__ct     = stol(items[15], nullptr, 10);
    process_data._cstime__ct     = stol(items[16], nullptr, 10);
    process_data._starttime__ct  = stol(items[21], nullptr, 10);
    process_data._utime__sec     = process_data._utime__ct/(float)sysconf(_SC_CLK_TCK);
    process_data._stime__sec     = process_data._stime__ct/(float)sysconf(_SC_CLK_TCK);
    process_data._cutime__sec    = process_data._cutime__ct/(float)sysconf(_SC_CLK_TCK);
    process_data._cstime__sec    = process_data._cstime__ct/(float)sysconf(_SC_CLK_TCK);
    process_data._starttime__sec = process_data._starttime__ct/(float)sysconf(_SC_CLK_TCK);
  }
}

void LinuxParser::updateProcessData_UidUser (string pid_str, ProcessData& process_data) {
  string uid_line =
    getLineStartingWith("Uid:", kProcDirectory + pid_str + kStatusFilename);
  if (!uid_line.empty()) {
    vector<string> items = split(uid_line);
    process_data._Uid = items[1];
    process_data._User = getUserNameFromUid(process_data._Uid);
  }
}

string LinuxParser:: getUserNameFromUid (string Uid) {
  string userName = "";
  string search_text = "x:" + Uid + ":" + Uid + ":";
  string line;
  std::ifstream filestream(kPasswordPath);
  if (filestream.is_open()) {
    while(std::getline(filestream, line)) {
      if (line.find(search_text) != string::npos)
        userName = getTextBeforeColon(line);
    }
  }
  return userName;
}

void LinuxParser::updateProcessData_Vm(int pid, ProcessData& process_data) {
  string line;
  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  if (filestream.is_open()) {
    while(std::getline(filestream, line)) {
      if("VmSize" == getTextBeforeColon(line)) 
        process_data._VmSize__kB = getkB(line);
      else if ("VmData" == getTextBeforeColon(line))
        process_data._VmData__kB = getkB(line);
    }
  }
}

void LinuxParser::updateSystemData_os_name(SystemData& system_data) {
  string line = getLineStartingWith("PRETTY_NAME", kOSPath);
  if (!line.empty()) {
    string s = line.substr(12);
    s.erase(remove( s.begin(), s.end(), '\"'), s.end());
    system_data._os_name = s;
  }
}

void LinuxParser::updateSystemData_kernel_version(SystemData& system_data) {
  string line = getFirstLineFromPath(kProcDirectory + kVersionFilename);
  system_data._kernel_version = getWordAt(line, 2);
}

void LinuxParser::updateSystem_PIDS(SystemData& system_data) {
  vector<int> temp_pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        temp_pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  system_data._pids = temp_pids;
}

void LinuxParser::updateSystem_mem_info(SystemData& system_data) {
  string line;
  std::ifstream filestream(kProcDirectory + kMeminfoFilename);
  if(filestream.is_open()) {
    while(std::getline(filestream, line)) {
      string name = getTextBeforeColon(line);
      if(name == "MemTotal")
        system_data._mem_total__kB = getkB(line);
      else if(name == "MemFree")
        system_data._mem_free__kB = getkB(line);
    }
  }
}

void LinuxParser::updateSystemData_up_time(SystemData& system_data) {
  system_data._up_time__sec = UpTime();
}

void LinuxParser::updateSystemData_kStatFilename(SystemData& system_data) {
  string line = getLineStartingWith("procs_running", kProcDirectory + kStatFilename);
  system_data._procs_running = stoi(getWordAt(line, 1), &sz);
}

long LinuxParser::UpTime() {
  string line = LinuxParser::getFirstLineFromPath(kProcDirectory + kUptimeFilename);
  string uptime_word = getWordAt(line, 0);
  if (!uptime_word.empty())
    return stol(uptime_word, nullptr, 10);
  return -1;
}

string LinuxParser::getTextBeforeColon(string str) {
  int j = str.find(':');
  return str.substr(0, j);
}

int LinuxParser::getkB(string str) {
  string number = regex_replace(str, regex("[^0-9]*([0-9]+).*"), string("$1"));
  return stoi(number, &sz);
}

vector<std::string> LinuxParser::split(string str) {
  vector<string> result;
  std::istringstream iss(str);
  for(string s; iss >> s;)
    result.push_back(s);
  return result;
}

string LinuxParser::getLineStartingWith(string word, string path) {
  std::ifstream filestream(path);
  string line;
  if(filestream.is_open()) {
    while( std::getline(filestream, line) ) {
      if (line.rfind(word, 0) == 0)
        return line;
    }
  }
  return "";
}

// First position is 0.
string LinuxParser::getWordAt (string text, int position) {
  if (text != "" && position >= 0) {
    vector<string> splitted = split(text);
    if ( position < static_cast<int>(splitted.size()) )
      return splitted[position];
  }
  return "";
}

string LinuxParser::getFirstLineFromPath(string path) {
  std::ifstream filestream(path);
  string line = "";
  if ( filestream.is_open() )
    std::getline(filestream, line);
  return line;
}
