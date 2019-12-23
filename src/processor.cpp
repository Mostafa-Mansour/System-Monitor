#include "processor.h"
#include "linux_parser.h"
#include "iostream"
#include "thread"
// TODO: Return the aggregate CPU utilization
float Processor::Utilization() {
    int numCpus=std::thread::hardware_concurrency(); 
    std::string line,uptime, idletime;
    std::ifstream stream(LinuxParser::kProcDirectory + LinuxParser::kUptimeFilename);
    if (stream.is_open()) {
      std::getline(stream, line);
      std::istringstream linestream(line);
      linestream >> uptime >> idletime;
    }

    float usageRatio=(numCpus*std::stol(uptime) - std::stol(idletime));
    //std::cout<<"Idle time is"<<std::stol(idletime)<<", uptime"<<std::stol(uptime)<<std::endl;
    //std::cout<<"All cpus:"<<numCpus*std::stol(uptime)<<", idel:"<<std::stol(idletime)<<std::endl;
    //std::cout<<"difference:"<< usageRatio/(numCpus*std::stol(uptime))<<std::endl;
    //std::cout<<"The percentage is:"<<(numCpus*std::stol(uptime) - std::stol(idletime))/(numCpus*std::stol(uptime))<<std::endl;
    return usageRatio/(numCpus*std::stol(uptime));
    
    }