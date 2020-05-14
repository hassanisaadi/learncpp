#include "ncurses_display.h"
#include "system.h"

#include <iostream>
#include <vector>
#include <string>
#include "linux_parser.h"
#include "format.h"

int main() {
  /*
  std::cout << LinuxParser::OperatingSystem() << "\n";
  std::cout << LinuxParser::Kernel() << "\n";

  std::vector<int> pids = LinuxParser::Pids();
  for (auto pid: pids){
    std::string uid = LinuxParser::Uid(pid);
    std::string username = LinuxParser::User(std::stoi(uid));
    long uptime = LinuxParser::UpTime(pid);
    std::string cmd = LinuxParser::Command(pid);
    std::string ram = LinuxParser::Ram(pid);
    std::cout << "pid = " << pid << ", ";
    std::cout << "Uid = " << uid << ", ";
    std::cout << "Username = " << username << ", ";
    std::cout << "Uptime = " << uptime << ", ";
    std::cout << "UPtime = " << Format::ElapsedTime(uptime) << ", ";
    std::cout << "RAM = " << ram << "MB\n";
    //std::cout << "cmd = " << cmd << ", ";
  }
  //std::cout << "Mem Utilization = " << 
  //  LinuxParser::MemoryUtilization() << " MB\n";
  std::cout << "Total Processes = " <<
    LinuxParser::TotalProcesses() << "\n";
  std::cout << "Running Processes = " <<
    LinuxParser::RunningProcesses() << "\n";
  std::cout << "System Uptime = " <<
    LinuxParser::UpTime() << " sec\n";
  */
  System system;
  NCursesDisplay::Display(system);
  
  //std::vector<int> pids = LinuxParser::Pids();
  //std::cout << pids[0] << "\n";
  //std::vector<std::string> a = LinuxParser::CpuUtilization(pids[0]);
}