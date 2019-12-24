#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <iostream>
#include <unistd.h>
#include <thread>

#include "linux_parser.h"


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
  string os, kernel, version;
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
  string line, title, usage;
  vector<string> memVec;
  float memTotal, memFree;
  int idx=0;
  std::ifstream stream(kProcDirectory + kMeminfoFilename);
    if (stream.is_open()) {
      while(std::getline(stream,line)){
        std::istringstream linestream(line);
        linestream>>title>>usage;
        if(title=="MemTotal:"){
          memTotal=std::stof(usage);
          ++idx;
          }
         
        else if (title=="MemFree:")
         
        {
           memFree=std::stof(usage);
           ++idx;
        }

        if(idx==2)
          break;
          

      } 
    float memUsage=memTotal-memFree;
    return memUsage/memTotal;   
    }
    return 0;
 }

// TODO: Read and return the system uptime
long LinuxParser::systemUpTime() { 
  string line,uptime;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
    if (stream.is_open()) {
      std::getline(stream, line);
      std::istringstream linestream(line);
      linestream >> uptime;
    }
    return std::stol(uptime);

  }

// TODO: Read and return the number of jiffies for the system
// long LinuxParser::Jiffies() { return systemUpTime() * sysconf(_SC_CLK_TCK); }

// long LinuxParser::ActiveJiffies(int pid) {
//   string line, token;
//   vector<string> values;
//   std::ifstream filestream(LinuxParser::kProcDirectory + to_string(pid) +
//                            LinuxParser::kStatFilename);
//   if (filestream.is_open()) {
//     std::getline(filestream, line);
//     std::istringstream linestream(line);
//     while (linestream >> token) {
//       values.push_back(token);
//     }
//   }
//   long jiffies{0};
//   if (values.size() > 21) {
//     long user = stol(values[13]);
//     long kernel = stol(values[14]);
//     long children_user = stol(values[15]);
//     long children_kernel = stol(values[16]);
//     jiffies = user + kernel + children_user + children_kernel;
//   }
//   return jiffies;
// }

// long LinuxParser::ActiveJiffies() {
//   vector<string> time = CpuUtilization();
//   return (stol(time[CPUStates::kUser_]) + stol(time[CPUStates::kNice_]) +
//           stol(time[CPUStates::kSystem_]) + stol(time[CPUStates::kIRQ_]) +
//           stol(time[CPUStates::kSoftIRQ_]) + stol(time[CPUStates::kSteal_]) +
//           stol(time[CPUStates::kGuest_]) + stol(time[CPUStates::kGuestNice_]));
// }

// long LinuxParser::IdleJiffies() {
//   vector<string> time = CpuUtilization();
//   return (stol(time[CPUStates::kIdle_]) + stol(time[CPUStates::kIOwait_]));
// }

// TODO: Read and return Process CPU utilization
float LinuxParser::processCpuUtilization(int pid) {
  string line, value;
  std::vector<std::string> utilValues;
  float utime, stime,cutime,starttime,cstime;
  std::string path=kProcDirectory +std::to_string(pid)+kStatFilename;
  //std::cout<<"The path is"<<path<<std::endl;
  std::ifstream stream(path);
  int idx=0;
  
   if (stream.is_open()) {
     std::getline(stream,line);
     std::istringstream linestream(line);
     while(idx<22){
       linestream>>value;
       utilValues.push_back(value);
       idx++;
     }
    utime=std::stof(utilValues[13]);
    stime=std::stof(utilValues[14]);
    cutime=std::stof(utilValues[15]);
    cstime=std::stof(utilValues[16]);
    starttime=std::stof(utilValues[21]);
    double hertz=sysconf(_SC_CLK_TCK);
    int numCpus=std::thread::hardware_concurrency();
    double uptime=numCpus*systemUpTime();

    float total_time = utime + stime;
    total_time = total_time + cutime + cstime;

    float seconds = uptime - (starttime / hertz);
    return   ((total_time / hertz) / seconds);
     
   }
   
  return 0;

 }

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() { 
  string line, title, value;
  
  std::ifstream stream(kProcDirectory + kStatFilename);
    //stream.close();
    if (stream.is_open()) {
      while(std::getline(stream,line)){
        std::istringstream linestream(line);
        linestream >> title >> value;
        if(title=="processes"){
          stream.close();
          return std::stoi(value);;
        }
          
      }
    }
    
    return 0;
    
    }

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() { 

  string line, title, value;
  
  std::ifstream stream(kProcDirectory + kStatFilename);
    if (stream.is_open()) {

      while(std::getline(stream,line)){
        std::istringstream linestream(line);
        linestream >> title >> value;
        if (title=="procs_running"){
          stream.close();
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
  std::string path=kProcDirectory +std::to_string(pid)+kCmdlineFilename;
  std::ifstream stream(path);
  string value="";
   if (stream.is_open()) {
     std::getline(stream,line);
     std::istringstream linestream(line);
     linestream>>value;
     stream.close();
     return value;
  
   }
  return std::string(); 
   
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) { 
  
  string line;
  std::string path=kProcDirectory +std::to_string(pid)+kStatusFilename;
  //std::cout<<"The path is"<<path<<std::endl;
  std::ifstream stream(path);
  string key="", value;
   if (stream.is_open()) {
     while(std::getline(stream,line)){
      std::istringstream linestream(line);
      linestream >> key >> value;
      if(key=="VmSize:")
        return value;

     }
   }
   return std::string();


}

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) { 
  string line, key, value;
  
  std::string path=kProcDirectory +std::to_string(pid)+kStatusFilename;
  //std::cout<<"The path is"<<path<<std::endl;
  std::ifstream stream(path);
  if (stream.is_open()) {
    while(std::getline(stream,line)){
      std::istringstream linestream(line);
      linestream >> key >> value;
      if(key=="Uid:")
        return value;
    }
  }
  return std::string();
}
 

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) { 
  string line,user,temp,uid;
  string userUid=Uid(pid);
  string path=kPasswordPath;
  std::ifstream stream(path);
  if(stream.is_open()){
    while(std::getline(stream, line)){
      std::replace(line.begin(),line.end(),':',' ');
      std::istringstream linestream(line);
      linestream>>user>>temp>>uid;
      if(uid==userUid){
        break;
      }


    }
    return user;
  }
return std::string();
 }

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) { 
  string line,value;
  std::string path=kProcDirectory +std::to_string(pid)+kStatFilename;
  //std::cout<<"The path is"<<path<<std::endl;
  std::ifstream stream(path);
  if(stream.is_open()){
    int idx=0;
    std::getline(stream,line);
    std::istringstream linestream(line);
    while(idx<22){
      
      linestream>>value;
      idx++;

    }
    return std::stol(value)/sysconf(_SC_CLK_TCK);
  }
  //std::cout<<"value:"<<value<<std::endl;
  
  return 0;

 }