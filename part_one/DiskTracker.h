#ifndef DISKTRACKER_H_INCLUDED
#define DISKTRACKER_H_INCLUDED

#include <string>

using namespace std;

class DiskTracker {

public:
    DiskTracker();

    void calculate(string);
    void print();

private:
    int total_memory_capacity = 0, total_free_memory = 0;
    int calculations = 0;
    float avg_free_memory;

    string mem_total_str, mem_free_str, tmpStr;
};

#endif