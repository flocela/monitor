#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <regex>

#include "linux_parser.h"

#include <iostream>

using std::cout;

using std::stof;
using std::string;
using std::to_string;
using std::vector;
using std::regex;
using std::stoi;
std::string::size_type sz;

namespace LinuxParser {
  string getStringBeforeColon(string str);
  int getkB(string str);
  vector<std::string> split(string str);
  string getLineStartingWith(string word, string path);
  string getWord(string text, int position);
  string getLineFromPath(string path);
}

// DONE: An example of how to read data from the filesystem
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

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, version, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
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

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() { 
  string line;
  std::ifstream filestream(kProcDirectory + kMeminfoFilename);
  int mem_total = 0;
  int mem_free  = 0;
  int buffers   = 0;
  int cached    = 0;
  int s_recl    = 0;
  int shmem     = 0;
  if(filestream.is_open()) {
    while(std::getline(filestream, line)) {
      string name = getStringBeforeColon(line);
      if(name == "MemTotal")
        mem_total = getkB(line);
      else if(name == "MemFree")
        mem_free = getkB(line);
      else if(name == "Buffers")
        buffers = getkB(line);
      else if (name == "Cached") {
        cached = getkB(line);
      }
      else if (name == "SReclaimable") {
        s_recl = getkB(line);
      }
      else if (name == "Shmem") {
        shmem = getkB(line);
      }
    }
  }

  int tot_mem = mem_total - mem_free;
  int cached_mem = cached + s_recl - shmem;
  int non_cached = tot_mem - buffers - cached_mem;

  return non_cached; 
}

// TODO: Read and return the system uptime
// In seconds
long LinuxParser::UpTime() {
  string uptime;
  string line;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> uptime;
    return stol(uptime, nullptr, 10);
  }
  else return 0l;
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies()  { 
  string cpu, user, nice, system;
  string line;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> cpu >> user >> nice >> system;
    return stol(system, nullptr, 10);
  }
  return 0l;
}

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid[[maybe_unused]]) { 
  string line;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
  if(stream.is_open()) {
    getline(stream, line);
    vector<string> items = split(line);
    long utime = stol(items[13], nullptr, 10);
    long stime = stol(items[14], nullptr, 10);
    long cutime = stol(items[15], nullptr, 10);
    long cstime = stol(items[16], nullptr, 10);
    return utime + stime + cutime + cstime;
  }
  return 0l; 
  }

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { 
  string line;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if(stream.is_open()) {
    getline(stream, line);
    vector<string> items = split(line);
    long user = stol(items[1], nullptr, 10);
    long nice = stol(items[2], nullptr, 10);
    long system = stol(items[3], nullptr, 10);
    return user + nice + system;
  }
  return 0; 
}

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { 
  string line;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if(stream.is_open()) {
    getline(stream, line);
    vector<string> items = split(line);
    long idle = stol(items[4], nullptr, 10);
    long iowait = stol(items[5], nullptr, 10);
    return idle + iowait;
  }
  return 0; 
}

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { 
  vector<string> result = {};
  string line;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if(stream.is_open()) {
    getline(stream, line);
    result = split(line);
    result.erase(result.begin(), result.begin() + 1);
  }
  return result; 
}

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  string line = getLineStartingWith("processes", kProcDirectory + kStatFilename);
  if (line != "") {
    string value = getWord(line, 1);
    if (value != "")
      return stoi(value, &sz);
  }
  return 0;
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() { 
  string line = getLineStartingWith("procs_running", kProcDirectory + kStatFilename);
  if (line != "") {
    string value = getWord(line, 1);
    if (value != "")
      return stoi(value, &sz);
  }
  return 0; 
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid[[maybe_unused]]) {
  string path = kProcDirectory + std::to_string(pid) + "/cmdline";
  return getLineFromPath(path);
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid[[maybe_unused]]) { 
  string line;
  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  if (filestream.is_open()) {
    while(std::getline(filestream, line)) {
      string name = getStringBeforeColon(line);
      if (name == "VmSize")
        return std::to_string(getkB(line));
    }
  }
  return string(); 
}

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid[[maybe_unused]]) {
  string line = getLineStartingWith("Uid:", kProcDirectory + std::to_string(pid) + kStatusFilename);
  if (!line.empty())
    return getWord(line, 1);
  return string(); 
}

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid[[maybe_unused]]) {
  string uid = Uid(pid);
  string lookfor = "x:" + uid + ":" + uid + ":";
  string line;
  std::ifstream filestream(kPasswordPath);
  if (filestream.is_open()) {
    while(std::getline(filestream, line)) {
      if (line.find(lookfor) != std::string::npos) {
        return getStringBeforeColon(line);
      }
    }
  }
  return string();
}

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
// In Seconds
long LinuxParser::UpTime(int pid[[maybe_unused]]) {
  string line;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
  if(stream.is_open()) {
    getline(stream, line);
    vector<string> items = split(line);
    long pidStartTime = stol(items[21], nullptr, 10);
    long upTime = UpTime(); // This is in seconds
    return ( upTime - (pidStartTime / sysconf(_SC_CLK_TCK)) );
  } 
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
string LinuxParser::getWord (string text, int position) {
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
