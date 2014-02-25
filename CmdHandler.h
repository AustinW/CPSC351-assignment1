#ifndef CMDHANDLER_H_INCLUDED
#define CMDHANDLER_H_INCLUDED

#define MAXLINE 1024
#define MAX_CMD_NUM 8
#define MAX_ARG_NUM 64

class CmdHandler {
	
public:
	CmdHandler();

	static bool isPipeBg(char *);
	static void deleteBgChar(char *);

	int parse(char input[MAXLINE]);
	int parseCommands(char *, char *arg_v[MAX_ARG_NUM]);

	char commands[MAX_CMD_NUM][MAXLINE];
	int child_pids[MAX_CMD_NUM];
	int child_status[MAX_CMD_NUM];

	int getNumCommands();

private:
	
	int num_commands;

};

#endif