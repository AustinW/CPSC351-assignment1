#ifndef CPUTRACKER_H_INCLUDED
#define CPUTRACKER_H_INCLUDED

#include <string>

using namespace std;

class CPUTracker {

public:
    CPUTracker();

    void calculate(string);
    void print();

private:
    int total_user_time, total_system_time, total_idle_time;
    int calculations = 0;
    float avg_user_time, avg_system_time, avg_idle_time;

    string user_mode_str, system_mode_str, idle_mode_str, tmpStr;
};

#endif