#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "include/history.h"
#include "include/global.h"

void newEntry(char* command){
	if (command_history_length >= command_history_max){

		free(command_history[0]);

		for (int i = 1; i < command_history_max; i++){
			command_history[i - 1] = command_history[i];
		}

		char* entry = malloc(sizeof(char) * strlen(command));
		strcpy(entry, command);
		command_history[command_history_max - 1] = entry;


	} else {

		char* entry = malloc(sizeof(char) * strlen(command));
		strcpy(entry, command);
		command_history[command_history_length] = entry;
		command_history_length ++;

	}
}