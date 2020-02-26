#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

// TODO: Return this process's ID
int Process::Pid() { 
    return pid_; 
    }

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() { 
  string line;
  float utime, stime, cutime, cstime, starttime;
  float uptime = LinuxParser::UpTime();
  float cpu_usage;
  std::ifstream stream(LinuxParser::kProcDirectory + to_string(pid_) + LinuxParser::kStatFilename);
  
  if (stream.is_open()){
    std::getline(stream, line);
    std::istringstream linestream(line);
    std::istream_iterator<string> beg(linestream), end;
    vector<string> values(beg,end);
    utime = std::stol(values[13]);
    stime = std::stol(values[14]);
    cutime = std::stol(values[15]);
    cstime = std::stol(values[16]);
    starttime = std::stol(values[21]);
    float total_time = utime + stime + cutime + cstime;
    float seconds = uptime - (starttime / sysconf(_SC_CLK_TCK));
    cpu_usage = ((total_time/sysconf(_SC_CLK_TCK) ) / seconds );
    return cpu_usage;
    }
    return 0;
    }

// TODO: Return the command that generated this process
string Process::Command() { 
    return LinuxParser::Command(pid_);
    }

// TODO: Return this process's memory utilization
string Process::Ram() { 
    return LinuxParser::Ram(pid_); 
    }

// TODO: Return the user (name) that generated this process
string Process::User() { 
    return LinuxParser::User(pid_);
    }

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() { 
    return LinuxParser::UpTime(pid_); 
    }

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a) const { 
    return this->ram_ < a.ram_;
    }