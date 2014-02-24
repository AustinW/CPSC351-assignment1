#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <memory.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <unistd.h>
#include <signal.h>
#include <iostream>

#include "CmdHandler.h"

using namespace std;

#define MAXLINE        1024    // Max length of a command
#define MAX_CMD_NUM    8   // Max number of commands in a pipeline
#define MAX_ARG_NUM    64  // Max number of arguments for each command
#define MAX_ARG_LENGTH 256  //Max lenth of each argument
#define WRITE_END      1
#define READ_END       0

int main() {
	fflush(stdout);

	printf("CPSC351$ ");

	char commandLine[MAXLINE];

	CmdHandler *cmdHandler = new CmdHandler();

	while (true) {
		if (cin.getline(commandLine, MAXLINE)) {

			int pipe_bg = CmdHandler::isPipeBg(commandLine);
			int num_commands = cmdHandler->parse(commandLine);

			if (strncmp(cmdHandler->commands[0], "exit", 4) == 0) {
				cout << "Exiting! Bye..." << endl;
				exit(0);
			} else if (strncmp(cmdHandler->commands[0], "cd", strlen("cd")) == 0) {
				
				char dir_name[MAX_ARG_LENGTH];
				char *arg_v[MAX_ARG_NUM];

				cmdHandler->parseCommands(dir_name, arg_v);
				chdir(arg_v[1]);
				continue;
			}
		} else {
			cerr << "Error reading command" << endl;
		}
	}

	return 0;
}