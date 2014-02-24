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