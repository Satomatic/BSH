#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>

#include "include/history.h"
#include "include/global.h"

void newEntry(char* command){
	/*
		Check that the command actually contains letters, not
		just whitespace.
	*/
	int f = 0;
	for (int i = 0; i < strlen(command); i++){
		if (isalpha(command[i]) ||
			ispunct(command[i]) ||
			isdigit(command[i])
		){
			f = 1;
			break;
		}
	}

	/*
		If the command is just white space then don't push it
		into the command history.
	*/
	if (f == 0){
		return;
	}

	/*
		If the command history is full then it should replace
		the last one after moving the rest up.
	*/
	if (command_history_length >= command_history_max){

		free(command_history[0]);

		for (int i = 1; i < command_history_max; i++){
			command_history[i - 1] = command_history[i];
		}

		char* entry = malloc(sizeof(char) * strlen(command));
		strcpy(entry, command);
		command_history[command_history_max - 1] = entry;


	/*
		Else it will just make a new entry into the history.
	*/
	} else {
		char* entry = malloc(sizeof(char) * strlen(command) + 1);
		memset(entry, 0, strlen(command) + 1);
		strcpy(entry, command);

		command_history[command_history_length] = entry;
		command_history_length ++;

	}
}
