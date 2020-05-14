#include <string>

#include <cmath>
#include <cassert>

#include "format.h"

using std::string;

// DONE: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
string Format::ElapsedTime(long seconds) {
    // without days
    int h = floor(seconds / 3600);
    int m = floor((seconds - h*3600) / 60);
    int s = (seconds - h*3600 - m*60) % 60;

    return ((h < 10)? "0" + std::to_string(h) : std::to_string(h)) + ":" +
           ((m < 10)? "0" + std::to_string(m) : std::to_string(m)) + ":" +
           ((s < 10)? "0" + std::to_string(s) : std::to_string(s));

    /*   //with days
    string out;
    int d = floor(seconds / (24*3600));
    int h = floor((seconds - d*24*3600) / 3600);
    int m = floor((seconds - d*24*3600 - h*3600) / 60);
    int s = seconds - d*24*3600 - h*3600 - m*60;
    out = std::to_string(d) + " Days, " + 
          std::to_string(h) + ":" + 
          std::to_string(m) + ":" +
          std::to_string(s);
    return out;
    */
}