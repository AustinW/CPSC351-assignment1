all: proc_parse my_shell

# Part One
proc_parse: part_one/proc_parse.cpp
	gcc part_one/proc_parse.cpp -o proc_parse -lstdc++ -std=c++11 -lpthread

# Part Two
my_shell: main.o functions.o
	g++ -o my_shell main.o functions.o; rm *.o
functions.o: part_two/functions.cpp part_two/general.h
	g++ -Wall -c part_two/functions.cpp
main.o: part_two/main.cpp part_two/general.h
	g++ -Wall -c part_two/main.cpp