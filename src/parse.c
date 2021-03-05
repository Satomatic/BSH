#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>

#include "include/parse.h"
#include "include/utils.h"

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

void newToken(char** array, char* token, int* position, int* tokenindex){
	// make copy of token
	char* temp = malloc(sizeof(char) * strlen(token));
	strcpy(temp, token);
	array[*position] = temp;

	// clear current token
	memset(token, 0, strlen(token));

	*tokenindex = 0;
	*position += 1;
}

char** splitInput(char* input){
	int buffersize = 64, position = 0;
	char** array = malloc(sizeof(char*) * buffersize);

	int tokenmax = 100, tokenindex = 0;
	char* token = malloc(sizeof(char) * tokenmax);
	bool stringMode = false;

	for (int i = 0; i < strlen(input); i++){
		if (input[i] == '$'){
			/*
				Get length of environment variable name
			*/
			int start = i + 1;
			int end = nextToken(input, start);

			/*
				Get environment variable
			*/
			char* envname = malloc(sizeof(char) * (end - start));
			strncpy(envname, input+start, end-start);
			char* value = getenv(envname);

			/*
				Copy the variable content into the token
			*/
			strcpy(token+tokenindex, value);
			tokenindex += strlen(value);
			i = end - 1;

			free(envname);
		
		} else if (input[i] == '*'){
			int start = i + 2;
			int end = nextToken(input, start);

			/*
				Get file extension from token
			*/
			char* fileext = malloc(sizeof(char) * (end - start) + 1);
			memset(fileext, 0, sizeof(char) * (end - start));
			strncpy(fileext, input+start, end-start);

			char* directory = token;

			/*
				If no directory specified, use current directory
			*/
			if (strlen(token) == 0){
				directory = "./";
			}

			/*
				For each file in directory
			*/
			char** listing = listdir(directory);

			int x = 0;
			while (listing[x] != NULL){
				const char* extension = getextension(listing[x]);
				
				/*
					If file has matching extension, insert as a new token
				*/
				if (strcmp(extension, fileext) == 0 && strlen(extension) > 0){
					newToken(array, listing[x], &position, &tokenindex);
				}

				x++;
			}

			i = end - 1;

			/*
				Clear the rest of the token
			*/
			memset(token, 0, tokenmax);
			tokenindex = 0;

			free(fileext);

		} else if (input[i] == '~'){
			char* home = getenv("HOME");
			strcpy(token+tokenindex, home);
			tokenindex += strlen(home);

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

		} else if (input[i] == ' ' && strlen(token) != 0){
			newToken(array, token, &position, &tokenindex);

		} else if (input[i] == '"'){
			stringMode = !stringMode;

		} else {
			token[tokenindex] = input[i];
			tokenindex ++;
		}
	}

	if (strlen(token) > 0){
		newToken(array, token, &position, &tokenindex);
	}

	array[position] = NULL;

	free(token);

	return array;
}