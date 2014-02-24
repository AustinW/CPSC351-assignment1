#include "CPUTracker.h"

#include <iostream>
#include <sstream>
#include <fstream>

using namespace std;

CPUTracker::CPUTracker() {}

void CPUTracker::calculate(string proc_stat) {

	ifstream statsFile(proc_stat);
	string line;

	if (getline(statsFile,  line)) {
	    
	    istringstream iss(line);

	    if ( ! (iss >> this->tmpStr
	    			>> this->user_mode_str
	    			>> this->tmpStr
	    			>> this->system_mode_str
	    			>> this->idle_mode_str)) {

	        cout << "ERROR: Corrupted /proc/stat" << endl;
	    } else {

	    	++this->calculations;

	        this->total_user_time   += stoi(this->user_mode_str);
	        this->total_system_time += stoi(this->system_mode_str);
	        this->total_idle_time   += stoi(this->idle_mode_str);

	        this->avg_user_time   = (float) this->total_user_time   / (float) this->calculations;
	        this->avg_system_time = (float) this->total_system_time / (float) this->calculations;
	        this->avg_idle_time   = (float) this->total_idle_time   / (float) this->calculations;
	    }
	}
}

void CPUTracker::print() {
	cout << "USER MODE: "     << this->avg_user_time;
	cout << "\tSYSTEM MODE: " << this->avg_system_time;
	cout << "\tIDLE MODE: "   << this->avg_idle_time << endl;
}

