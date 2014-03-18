#include <string>
#include <sys/types.h>
#include <sys/wait.h>
#include <cerrno>
#include <unistd.h>
#include <signal.h>
#include <iostream>
#include <fstream>

#include "general.h"

using namespace std;

#define MAXLINE        1024    // Max length of a command
#define MAX_CMD_NUM    8       // Max number of commands in a pipeline
#define MAX_ARG_NUM    64      // Max number of arguments for each command
#define MAX_ARG_LENGTH 256     //Max lenth of each argument
#define WRITE_END      1
#define READ_END       0

#define DEBUG true

void sigint_handler(int signum) {}
void sigtstp_handler(int signum) {}
// void evalCommand(CmdHandler *);

int main() {

	signal(SIGINT, sigint_handler); // ctrl-c
	signal(SIGTSTP, sigtstp_handler); // ctrl-z

	string prompt = "(CPSC351):";
	char *argv[MAX_ARG_NUM], *command1[MAX_ARG_NUM], *command2[MAX_ARG_NUM];
	RedirectPipe redirect_pipe;
	int argc;

	// Keep redirecting the user unless they terminate using specified keywords (exit)
	while (true) {

		cout << prompt << get_current_dir_name() << "$ ";

		argc = read_arguments(argv);
		//cout << "Num args: " << argc << endl;

		// Parse the command and split it if it needs to be split into command1 and command2
		// arrays. Indicate if the command pipes/redirects/neither
		redirect_pipe = parse_command(argc, argv, command1, command2);

		// Determine handling of the user's command(s)
		if (redirect_pipe == PIPE) {
			//cout << "Regular Pipe" << endl;
			pipe_command(command1, command2);
		} else if (redirect_pipe == REDIRECT) {
			//cout << "Redirect Pipe" << endl;
			redirect_command(command1, command2);
		} else {
			//cout << "Run Command" << endl;
			run_command(argc, argv);
		}

		// Reset the argv array for next use
		for (int i = 0; i < argc; ++i) argv[i] = NULL;

	}

	return 0;
}