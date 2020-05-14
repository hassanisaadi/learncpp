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

// DONE: Return this process's ID
int Process::Pid() const { return pid_; }

// DONE: Return this process's CPU utilization
float Process::CpuUtilization() const {
    vector<string> params = LinuxParser::CpuUtilization(Pid());
    if(params.size() == 0)
        return 0.0;
    long uptime = stol(params[0]);
    long utime = stol(params[1]);
    long stime = stol(params[2]);
    long cutime = stol(params[3]);
    long cstime = stol(params[4]);
    long starttime = stol(params[5]);
    long hertz = stol(params[6]);

    float total_time = (float)(utime + stime + cutime + cstime);
    float seconds = (float)uptime - ((float)starttime / (float)hertz);

    return (seconds < 0.00001)? 0.0 : (total_time / (float) hertz) / seconds;
}

// DONE: Return the command that generated this process
string Process::Command() { return LinuxParser::Command(Pid()); }

// DONE: Return this process's memory utilization
string Process::Ram() const{ return LinuxParser::Ram(Pid()); }

// DONE: Return the user (name) that generated this process
string Process::User() { return LinuxParser::User(Pid()); }

// DONE: Return the age of this process (in seconds)
long int Process::UpTime() { return LinuxParser::UpTime(Pid()); }

// DONE: Overload the "less than" comparison operator for Process objects
bool Process::operator<(Process const& a) const {
    //return this->Pid() < a.Pid();
    //return this->CpuUtilization() > a.CpuUtilization();
    return this->cpu_util_ > a.cpu_util_;
    //return this->Ram() > a.Ram();
}