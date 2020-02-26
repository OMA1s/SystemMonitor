#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>

#include "linux_parser.h"
using std::stoi;
using std::stof;
using std::string;
using std::to_string;
using std::vector;

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
  string os, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> kernel;
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
  string memTotal = "MemTotal:";
  string memFree = "MemFree:";
  string buffers = "Buffers:";
  float total_memory;
  float free_memory;
  float buffer_memory;
  std::ifstream stream(kProcDirectory + kMeminfoFilename);
  if (stream.is_open()){
    while(std::getline(stream, line)){
    
      if (line.compare(0, memTotal.size(), memTotal) == 0){
        std::istringstream linestream(line);
        std::istream_iterator<string> beg(linestream), end;
        vector<string> values(beg,end);
        total_memory = stof(values[1]);
        }
    
      if (line.compare(0, memFree.size(), memFree) == 0){
        std::istringstream linestream(line);
        std::istream_iterator<string> beg(linestream), end;
        vector<string> values(beg,end);
        free_memory = stof(values[1]);
        }
    
      if (line.compare(0, buffers.size(), buffers) == 0){
        std::istringstream linestream(line);
        std::istream_iterator<string> beg(linestream), end;
        vector<string> values(beg,end);
        buffer_memory = stof(values[1]);
        }
      }
    }
  return 1.0 - (free_memory / float(total_memory - buffer_memory) );
  }

// TODO: Read and return the system uptime

long LinuxParser::UpTime() {
  string path = kProcDirectory + kUptimeFilename;
  std::ifstream stream(path);
  string line;
  std::getline(stream, line);
  std::istringstream buffer(line);
  std::istream_iterator<string> begin(buffer), end;
  vector<string> line_content(begin, end);
  return stoi(line_content[0]);
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { 
  return ActiveJiffies() + IdleJiffies();
  
  }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid[[maybe_unused]]) { return 0; }

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { 
  vector <string> values = LinuxParser::CpuUtilization();
  return std::stol(values[LinuxParser::kUser_]) + std::stol(values[LinuxParser::kNice_]) +
    std::stol(values[LinuxParser::kSystem_]) + std::stol(values[LinuxParser::kIRQ_]) + 
    std::stol(values[LinuxParser::kSoftIRQ_]) + std::stol(values[LinuxParser::kSteal_]) + 
    std::stol(values[LinuxParser::kGuest_]) + std::stol(values[LinuxParser::kGuestNice_]); 
  }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { 
  vector <string> values = LinuxParser::CpuUtilization();
  return std::stol(values[LinuxParser::kIOwait_]) + std::stol(values[LinuxParser::kIdle_]);
  }

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { 
  string line;
  std::ifstream stream(kProcDirectory + kStatFilename);

    std::getline(stream, line);
    std::istringstream linestream(line);
    std::istream_iterator<string> beg(linestream), end;
    vector<string> values(beg,end);
    return values;
  }

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() { 
  string line;
  string name, value;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()){
    while (std::getline(stream, line)){
      std::istringstream linestream(line);
      linestream>>name>>value;
      if (name == "processes"){
        return std::stoi(value);
      }
    }
  }
  return 0;
  }

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() { 
  string line;
  string name, value;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()){
    while (std::getline(stream, line)){
      std::istringstream linestream(line);
      linestream>>name>>value;
      if (name == "procs_running"){
        return std::stoi(value);
      }
    }
  }
  return 0; 
  }

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) { 
  string line;
  string name, value;
  std::ifstream stream(kProcDirectory + to_string(pid) + kCmdlineFilename);
  if (stream.is_open()){
    std::getline(stream, line);
    return line;
  }
  return string();
  }

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) { 
  string line;
  string name, value;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (stream.is_open()){
    while(std::getline(stream, line)){
      std::istringstream linestream(line);
      linestream>>name>>value;
      if (name == "VmSize:")return to_string(std::stoi(value)/1000);
      }
  }
  return string(); 
}

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) { 
  string line;
  string name, value;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (stream.is_open()){
    while(std::getline(stream, line)){
      std::istringstream linestream(line);
      linestream>>name>>value;
      if (name == "Uid:") return value;
      }
    }
  return string();
  }

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) { 
  string line;
  string uId = LinuxParser::Uid(pid);
  std::ifstream stream(kPasswordPath);
  if (stream.is_open()){
    while(std::getline(stream, line)){
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      std::istream_iterator<string> beg(linestream), end;
      vector<string> values(beg,end);
      if (values[1] == "x" && values[2] == uId) { return values[0];} 
      }
    }
  return string();
  }

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) { 
  string line;
  string value;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
  
  if (stream.is_open()){
    std::getline(stream, line);
    //we need 22nd value from line
    std::istringstream linestream(line);
    std::istream_iterator<string> beg(linestream), end;
    vector<string> values(beg,end);
    return LinuxParser::UpTime() - (std::stof(values[21]) / sysconf(_SC_CLK_TCK));
    }
  return 0; 
  }