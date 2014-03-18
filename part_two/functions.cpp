#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <cerrno>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>

#include "general.h"

RedirectPipe parse_command(int argc, char **argv, char **command1, char **command2)
{
	RedirectPipe result = NONE;

	int split = -1;

	for (int i = 0; i < argc; ++i) {

		if (strcmp(argv[i], "|") == 0) {
			result = PIPE;
			split = i;
		} else if (strcmp(argv[i], ">>") == 0) {
			result = REDIRECT;
			split = i;
		}
	}

	if (result != NONE) {

		for (int i = 0; i < split; ++i) {
			command1[i] = argv[i];
		}

		int count = 0;
		for (int i = split + 1; i < argc; ++i) {
			command2[count] = argv[i];
			++count;
		}

		// execvp expects NULL terminated command strings
		command1[split] = NULL;
		command2[count] = NULL;
	}

	return result;
}

void pipe_command(char **command1, char **command2) {
	int file_descriptors[2];

	pipe(file_descriptors);
	pid_t process_id;

	// child process
	if (fork() == 0) {

		// Reassign stdin to file_descriptors[0] end of pipe
		dup2(file_descriptors[0], 0);

		// We aren't writing in the child process, closing pipe...
		close(file_descriptors[1]);

		// Execute the 2nd command
		execvp(command2[0], command2);
		perror("execvp command has failed");

	} else if ((process_id = fork()) == 0) {

		// Reassign stdout to file_descriptors[1] end of pipe
		dup2(file_descriptors[1], 1);

		// We aren't writing in the child process, closing pipe...
		close(file_descriptors[0]);

		// Execute the first command
		execvp(command1[0], command1);
		perror("execvp command has failed");

	} else {

		waitpid(process_id, NULL, 0);

	}
}

// Get input from the user, split the input into arguments, insert the arguments
// into the array, and return number of args as an integer
int read_arguments(char **argv)
{
	char *c_string;
	string argument;
	int argc = 0;

	// Read arguments until the user hits enter
	while (cin >> argument) {

		// Allow termination
		if (trying_to_quit(argument)) {
			cout << "Bye..." << endl;
			exit(0);
		}

		// Conver the string to a c string
		c_string = new char[argument.size() + 1];
		strcpy(c_string, argument.c_str());
		argv[argc] = c_string;

		// Increment the counter
		++argc;

		// If the user hit enter, stop reading
		if (cin.get() == '\n') break;
	}

	// Play nice with execvp
	argv[argc] = NULL;

	return argc;
}

void redirect_command(char **command, char **file) {

	int file_descriptors[2], count, file_descriptor;
	char c;
	pid_t process_id; // This will hold the process id. Useful with fork()

	pipe(file_descriptors);

	// Child process 1
	if (fork() == 0) {
		// http://linux.die.net/man/2/open
		file_descriptor = open(file[0], O_RDWR | O_CREAT, 0666);

		if (file_descriptor < 0) {
			cout << "Error: " << strerror(errno) << endl;
			return;
		}

		dup2(file_descriptors[0], 0);

		// Don't need stdout, end of pipe
		close(file_descriptors[0]);

		// Read from stdout
		while ((count = read(0, &c, 1)) > 0)
			write(file_descriptor, &c, 1); // write to file

		exit(0);

	} else if ((process_id = fork()) == 0) {

		dup2(file_descriptors[1], 1);

		// Don't need stdin end of pipe
		close(file_descriptors[0]);

		// Output contents to stdout
		execvp(command[0], command);
		perror("execvp command has failed");
	} else {

		// Parent process
		waitpid(process_id, NULL, 0);
		close(file_descriptors[0]);
		close(file_descriptors[1]);
	}
}

void run_command(int argc, char **argv)
{
	pid_t process_id;
	const char *ampersand = "&";
	bool found_ampersand = false;

	// If we find an ampersand as the last argument, let's set a flag
	if (strcmp(argv[argc - 1], ampersand) == 0) found_ampersand = true;

	// Fork our process
	process_id = fork();

	// error
	if (process_id < 0) {
		perror("Error, invalid process id");
	} else if (process_id == 0) {
		if (found_ampersand) {
			argv[argc - 1] = NULL;
			argc--;
		}

		execvp(argv[0], argv);
		perror("execvp command has failed");
		
	} else if ( ! found_ampersand) {
		waitpid(process_id, NULL, 0);
	}

}

bool trying_to_quit(string choice) {

	for (unsigned int i = 0; i < choice.length(); ++i) {
		choice[i] = tolower(choice[i]);
	}

	return (choice == "exit");
}