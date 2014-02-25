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

void evalCommand(CmdHandler *);

void sigint_handler(int signum) {}
void sigtstp_handler(int signum) {}

int main() {

	signal(SIGINT, sigint_handler); // ctrl-c
	signal(SIGTSTP, sigtstp_handler); // ctrl-z

	CmdHandler *cmdHandler = new CmdHandler();

	while (true) {

		char commandLine[MAXLINE];

		fflush(stdout);

		printf("CPSC351$ ");

		if (fgets(commandLine, MAXLINE, stdin) != NULL) {

			int pipe_bg = CmdHandler::isPipeBg(commandLine);
			if (pipe_bg) {
				CmdHandler::deleteBgChar(commandLine);
			}

			cmdHandler->parse(commandLine);

			if (strncmp(cmdHandler->commands[0], "exit", 4) == 0) {
				cout << "Exiting! Bye..." << endl;
				exit(0);
			} else if (strncmp(cmdHandler->commands[0], "cd", strlen("cd")) == 0) {
				
				char dir_name[MAX_ARG_LENGTH];
				char *arg_v[MAX_ARG_NUM];

				cmdHandler->parseCommands(dir_name, arg_v);

				chdir(arg_v[1]);
				continue;
			} else {
				evalCommand(cmdHandler);
				continue;
			}
		} else {
			cerr << "Error reading command" << endl;
		}
	}

	return 0;
}

void evalCommand(CmdHandler *cmdHandler)
{
	int group_id = 0;

	for (int i = 0; i < cmdHandler->getNumCommands(); ++i) {

		int old_pipe[2], new_pipe[2];
		pid_t pid;
		pipe(new_pipe);
		pid = fork();
		cmdHandler->child_pids[i] = pid;

		if (i == 0) group_id = pid; // track the first child's process id

		if (pid > 0) {
			// Parent process

			close(new_pipe[WRITE_END]);
			old_pipe[READ_END] = new_pipe[READ_END];
			old_pipe[WRITE_END] = new_pipe[WRITE_END];
		} else if (pid == 0) {
			// Child process

			if (setgid(group_id) == -1) {
				// printf("setgid Error: %s\n", strerror(errno));
			}

			close(new_pipe[READ_END]);

			if (i != 0) {
				// If this is not the first command, redirect the child stdin to old_pipe read

				dup2(old_pipe[READ_END], 0);
				close(old_pipe[READ_END]);
			}

			if (i != (cmdHandler->getNumCommands() - 1)) {
				// If this is not the last command, redirect child stdout to new_pipe write
				dup2(new_pipe[WRITE_END], 1);
				close(new_pipe[WRITE_END]);
			}

			char name[MAX_ARG_LENGTH];
			char *arg_v[MAX_ARG_NUM];
			int number_of_arguments = cmdHandler->parseCommands(name, arg_v);
			execvp(name, arg_v);
		} else {
			perror("ERROR: Could not create child process.");
		}
	}
}

