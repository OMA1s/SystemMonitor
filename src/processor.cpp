#include "processor.h"
#include "linux_parser.h"

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() { 
  float value = (float(LinuxParser::ActiveJiffies()) / float(LinuxParser::Jiffies()) );
  return value;
}