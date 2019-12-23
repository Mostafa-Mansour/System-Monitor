#include <string>

#include "format.h"

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) { 
    std::string upTime;
    int hours=seconds/(3600);
    long rest_minutes=seconds%3600;
    int minutes=rest_minutes/60;
    int sec=rest_minutes%60;

    return string(std::to_string(hours)+":"+std::to_string(minutes)+":"+std::to_string(sec));
    
     }