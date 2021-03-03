#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>

#include "include/parse.h"
#include "include/utils.h"

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

int nextToken(char* input, int start){
	int end = strlen(input);

	for (int i = start; i < strlen(input); i++){
		if (!isalpha(input[i])){
			end = i;
			break;
		}
	}

	return end;
}

char** splitInput(char* input){
	int buffersize = 64, position = 0;
	char** array = malloc(sizeof(char*) * buffersize);

	int tokenmax = 100, tokenindex = 0;
	char* token = malloc(sizeof(char) * tokenmax);
	bool stringMode = false;

	for (int i = 0; i < strlen(input); i++){
		if (input[i] == '$'){
			// get length of env variable name
			int start = i + 1;
			int end = nextToken(input, start);

			char* envname = malloc(sizeof(char) * (end - start));
			strncpy(envname, input+start, end-start);
			char* value = getenv(envname);

			// copy into the token
			strcpy(token+tokenindex, value);
			tokenindex += strlen(value);
			i = end - 1;

			free(envname);
		
		} else if (input[i] == '*'){
			int start = i + 2;
			int end = nextToken(input, start);

			// get file extension from token
			char* fileext = malloc(sizeof(char) * (end - start) + 1);
			memset(fileext, 0, sizeof(char) * (end - start));
			strncpy(fileext, input+start, end-start);

			char* directory = token;

			// if no directory, give current
			if (strlen(token) == 0){
				directory = "./";
			}

			// get directory listing
			char** listing = listdir(directory);

			int x = 0;
			while (listing[x] != NULL){
				
				const char* extension = getextension(listing[x]);

				// if file has matching extension, insert as token
				if (strcmp(extension, fileext) == 0 && strlen(extension) > 0){
					char* temp = malloc(sizeof(char) * strlen(listing[x]));
					strcpy(temp, listing[x]);

					array[position] = temp;
					position ++;
				}

				x++;
			}

			i = end - 1;

			// clear the rest of the token as we are done with it
			memset(token, 0, strlen(token));
			tokenindex = 0;
			
			free(fileext);

		} else if (input[i] == '\\'){
			if (
				input[i + 1] == ' '  ||
				input[i + 1] == '"'  ||
				input[i + 1] == '\\'
			){
				token[tokenindex] = input[i + 1];
				tokenindex ++;
				i++;
			}

		} else if (input[i] == ' ' && stringMode == true){
			token[tokenindex] = ' ';
			tokenindex ++;

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

		} else if (input[i] == '"'){
			stringMode = !stringMode;

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