#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "include/parse.h"

char** parseInput(char** array){
	int i = 0;
	while (array[i] != NULL){
		if (array[i][0] == '~'){
			char* home = getenv("HOME");

			char* newItem = malloc(sizeof(char) * (strlen(&array[i][0]) + strlen(home)));
			strcpy(newItem, home);
			strcpy(newItem + strlen(home), array[i]+1);

			free(array[i]);
			array[i] = newItem;
		}

		i++;
	}

	return array;
}

char** splitInput(char* input){
	int buffersize = 64, position = 0;
	char** array = malloc(sizeof(char*) * buffersize);

	int tokenmax = 100, tokenindex = 0;
	char* token = malloc(sizeof(char) * tokenmax);

	for (int i = 0; i < strlen(input); i++){
		if (input[i] == '\\' && input[i + 1] == ' '){
			token[tokenindex] = ' ';
			tokenindex ++;

			i++;

		} else if (input[i] == ' '){
			if (strlen(token) == 0) continue;

			// make copy of token
			char* temp = malloc(sizeof(char) * strlen(token));
			strcpy(temp, token);
			array[position] = temp;
			position ++;

			// clear current token
			memset(token, 0, strlen(token));
			tokenindex = 0;

		} else {
			token[tokenindex] = input[i];
			tokenindex ++;
		}
	}

	if (strlen(token) > 0){
		// make copy of token
		char* temp = malloc(sizeof(char) * strlen(token));
		strcpy(temp, token);
		array[position] = temp;
		position ++;

		// clear current token
		memset(token, 0, strlen(token));
		tokenindex = 0;
	}

	array[position] = NULL;

	free(token);

	return array;
}