#include "CmdHandler.h"
#include <string.h>

CmdHandler::CmdHandler() {
	
}

bool CmdHandler::isPipeBg(char *commandLine) {
	
	for (int i = strlen(commandLine) - 1; i >= 0; --i){
		
		char interpreter = commandLine[i];
		
		if (interpreter == '&') {
			return 1;
			break;
		}

	}

	return 0;
}

void CmdHandler::deleteBgChar(char *instruction) {
	
	for(int i = strlen(instruction) - 1; i >= 0; --i){
		
		char interpreter = instruction[i];
		
		if (interpreter == '&') {
			instruction[i] = ' ';
			break;
		}
	}
}

int CmdHandler::parse(char input[MAXLINE]) {

	char localInput[MAXLINE];
	strcpy(localInput, input);

	char *token, *restOfInput, *pointer = localInput;
	int count = 0;

	while (token = strtok_r(pointer, "\n|", &restOfInput)) {
		pointer = restOfInput;

		strcpy(this->commands[count++], token);
	}

	return count;
}

void CmdHandler::parseCommands(char *command, char *name, char *arg_v[MAX_ARG_NUM]) {
	char localCopy[MAXLINE];
	strcpy(localCopy, command);

	char *pointer = localCopy,
		 *restOfInput,
		 *token;

	int count = 0;

	while (token = strtok_r(pointer, "\t' ", &restOfInput)) {
		arg_v[count] = (char *)malloc(strlen(token)*sizeof(char));
		strcpy(arg_v[count++], token);
		pointer = restOfInput;
	}

	strcpy(name, arg_v[0]);
	arg_v[count] = NULL;
	return count;

}