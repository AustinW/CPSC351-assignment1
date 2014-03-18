my_shell: main.o functions.o
	g++ -o my_shell main.o functions.o; rm *.o
functions.o: functions.cpp general.h
	g++ -Wall -c functions.cpp
main.o: main.cpp general.h
	g++ -Wall -c main.cpp

# CC=g++
# CFLAGS=-c -Wall
# LDFLAGS=
# SOURCES=proc_parse.cpp general.h main.cpp functions.cpp
# OBJECTS=$(SOURCES:.cpp=.o)
# EXECUTABLE=hello

# all: $(SOURCES) $(EXECUTABLE)
	
# $(EXECUTABLE): $(OBJECTS) 
# 	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

# .cpp.o:
# 	$(CC) $(CFLAGS) $< -o $@