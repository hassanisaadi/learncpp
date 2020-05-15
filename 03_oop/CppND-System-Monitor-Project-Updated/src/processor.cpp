#include "processor.h"

#include <vector>
#include <string>
#include "linux_parser.h"
#include <chrono>
#include <thread>

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() {
    std::vector<std::string> prev_params = LinuxParser::CpuUtilization();
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    std::vector<std::string> params = LinuxParser::CpuUtilization();
    long prev_user, prev_nice, prev_system, prev_idle, prev_iowait, 
        prev_irq, prev_softirq, prev_steal;//, prev_guest, prev_guest_nice;
    long user, nice, system, idle, iowait, 
        irq, softirq, steal;//, guest, guest_nice;
    user = std::stol(params[0]);    prev_user = std::stol(prev_params[0]);
    nice = std::stol(params[1]);    prev_nice = std::stol(prev_params[1]);
    system = std::stol(params[2]);  prev_system = std::stol(prev_params[2]);
    idle = std::stol(params[3]);    prev_idle = std::stol(prev_params[3]);
    iowait = std::stol(params[4]);  prev_iowait = std::stol(prev_params[4]);
    irq = std::stol(params[5]);     prev_irq = std::stol(prev_params[5]);
    softirq = std::stol(params[6]); prev_softirq = std::stol(prev_params[6]);
    steal = std::stol(params[7]);   prev_steal = std::stol(prev_params[7]);

    long prev_Idle = prev_idle + prev_iowait;
    long prev_NonIdle = prev_user + prev_nice + prev_system +
             prev_irq + prev_softirq + prev_steal;
    long prev_Total = prev_Idle + prev_NonIdle;

    long Idle = idle + iowait;
    long NonIdle = user + nice + system + irq + softirq + steal;
    long Total = Idle + NonIdle;
    
    //differentiate: actual value minus the previous one
    long totald = Total - prev_Total;
    long idled = Idle - prev_Idle;
    if (totald != 0)
        return (float) (totald - idled) / (float) totald;
    else
        return 0.0;
}