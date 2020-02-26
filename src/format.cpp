#include <string>

#include "format.h"
using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds[[maybe_unused]]) { 
    long hours = seconds/3600.0;
    long minutes = ((seconds/3600.0) - hours) * 60;
    long secs = (((seconds/3600.0 - hours) * 60) - minutes) * 60;
    return std::to_string(hours) + ":" + std::to_string(minutes) + ":" + std::to_string(secs);
    }