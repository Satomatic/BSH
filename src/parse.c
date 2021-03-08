#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>

#include "include/parse.h"
#include "include/utils.h"

/*
	Split input at '&' to execute multiple
	commands in one line
*/
char** splitCommand(char* input){
	char** array = malloc(sizeof(char*));
	char* citem = malloc(1);
	
	int arraypos = 0;
	int citempos = 0;
	int i = 0;

	//for (int i = 0; input[i] != '\0'; i++){
	while (1){
		if (input[i] == '\\' && input[i + 1] == '&'){
			citem = realloc(citem, citempos + 1);
			citem[citempos] = input[i + 1];
			citempos ++;

			i ++;
	
		} else if (input[i] == '&' || (input[i] == '\0' && citempos > 0)){
			/*
				Copy item into new string
			*/
			char* item = malloc(strlen(citem) + 1);
			memset(item, 0, strlen(citem) + 1);
			strcpy(item, citem);

			/*
				Allocate new slot in array
			*/
			array = realloc(array, sizeof(char*) * (arraypos + 2));
			array[arraypos] = item;
			arraypos ++;

			/*
				Free current item
			*/
			free(citem);
			citem = malloc(1);
			citempos = 0;

		} else {
			citem = realloc(citem, citempos + 1);
			citem[citempos] = input[i];
			citempos ++;
		}

		if (input[i] == '\0') break;

		i++;
	}

	/*
		Last item in an array should always be NULL
	*/
	array[arraypos] = NULL;

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
	int tokenmax = 100, tokenindex = 0;

	char** array = malloc(sizeof(char*) * buffersize);
	char* token = malloc(sizeof(char) * tokenmax);
	memset(token, 0, tokenmax);

	bool stringMode = false;
	bool foundChar = false;

	for (int i = 0; i < strlen(input); i++){
		if (foundChar == false){
			if (isalpha(input[i]) ||
				ispunct(input[i]) ||
				isdigit(input[i])
			){
				foundChar = true;
			} else {
				continue;
			}
		}
		
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
			foundChar = false;

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
