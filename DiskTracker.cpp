#include "MemTracker.h"

#include <iostream>
#include <sstream>
#include <fstream>

using namespace std;

MemTracker::MemTracker() {}

void MemTracker::calculate(string proc_mem) {

	ifstream memFile(proc_mem);
	string line;

	// Only need to read the total the first time, but need to skip past the line
	// in all subsequent calls
	if (getline(memFile, line)) {

		if (this->total_memory_capacity == 0) {
			istringstream iss(line);

			if ( ! (iss >> this->tmpStr >> this->mem_total_str)) {
				cerr << "ERROR: Corrupted " << proc_mem << endl;
				return;
			} else {
				this->total_memory_capacity = stoi(this->mem_total_str);
			}
		}
	}

	if (getline(memFile, line)) {
		istringstream iss(line);

		if ( ! (iss >> this->tmpStr >> this->mem_free_str)) {
			cerr << "ERROR: Corrupted " << proc_mem << endl;
			this->mem_free_str = "";
		}
	}

	this->total_free_memory += stoi(this->mem_free_str);
	++this->calculations;

	this->avg_free_memory = ((float) this->total_free_memory / (float) this->calculations) / this->total_memory_capacity;
}

void MemTracker::print() {
	cout << "FREE MEMORY: " << (this->avg_free_memory * 100) << "%" << endl;
}

