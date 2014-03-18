#include <iostream>
#include <fstream>
#include <sys/stat.h>

using namespace std;

const int MAXIMUM_ARGS = 256;

enum RedirectPipe {PIPE, REDIRECT, NONE};

// Split the user's command into 2 commands, or command + file name
RedirectPipe parse_command(int, char **, char **, char **);

// Pipe the 1st command's output into the other command
void pipe_command(char **, char **);

// Read input from user and place in the given array.
// Return number of arguments read
int read_arguments(char **);

// Redirect the output from the command into the file
void redirect_command(char **, char **);

// Provided a number of arguments and an array of arguments, execute
// those arguments. First should be a command
void run_command(int, char **);

// Provided a string of user input, determine whether the user
// wants to quit the shell
bool trying_to_quit(string);