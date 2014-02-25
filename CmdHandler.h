#ifndef CMDHANDLER_H_INCLUDED
#define CMDHANDLER_H_INCLUDED

#define MAXLINE 1024
#define MAX_CMD_NUM 8

class CmdHandler {
	
public:
	CmdHandler();

	static bool isPipeBg(char *);
	static void deleteBgChar(char *);

	int parse(char input[MAXLINE]);
	int parseCommands(char *, char *, char *arg_v[MAX_ARG_NUM]);

	char commands[MAX_CMD_NUM][MAXLINE];

private:
	

};

#endif