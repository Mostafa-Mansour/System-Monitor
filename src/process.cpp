#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>
#include <iomanip>

#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;


Process::Process(int pid):_pid(pid){}
// TODO: Return this process's ID
int Process::Pid() { return _pid; }

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() const { 
    return LinuxParser::processCpuUtilization(_pid);
 }

// TODO: Return the command that generated this process
string Process::Command() { return LinuxParser::Command(_pid); }

// TODO: Return this process's memory utilization
string Process::Ram() { 
    string memUsage=LinuxParser::Ram(_pid);
    int memMb=0;
    std::setprecision(1);
    memMb=std::stoi(memUsage)/1024;
    return std::string(std::to_string(memMb)); 
    //return string(); 
    }

// TODO: Return the user (name) that generated this process
string Process::User() { return LinuxParser::User(_pid); }

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() { return LinuxParser::UpTime(_pid); }

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a) const { 

    return (this->CpuUtilization() > a.CpuUtilization());
 }