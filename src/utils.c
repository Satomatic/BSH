#include <termios.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "include/history.h"
#include "include/global.h"
#include "include/utils.h"

char **splitline(char *line){
	int bufsize = 64, position = 0;
	char **tokens = malloc(bufsize * sizeof(char*));
	char *token;

	if (!tokens) {
		fprintf(stderr, "lsh: allocation error\n");
		exit(EXIT_FAILURE);
	}

	token = strtok(line, " \t\r\n\a");
	while (token != NULL) {
		tokens[position] = token;
		position++;

		if (position >= bufsize) {
			bufsize += 64;
			tokens = realloc(tokens, bufsize * sizeof(char*));
			if (!tokens) {
				fprintf(stderr, "lsh: allocation error\n");
				exit(EXIT_FAILURE);
			}
		}

		token = strtok(NULL, " \t\r\n\a");
	}
	tokens[position] = NULL;
	return tokens;
}
/*
	This function is kinda long and smelly but it'll do
*/
char* getInput(int buffer_size){
	char* bufferStore = malloc(sizeof(char) * buffer_size);
	char* buffer = malloc(sizeof(char) * buffer_size);

	for (int i = 0; i < buffer_size; i++){
		buffer[i] = '\0';
		bufferStore[i] = '\0';
	}
	
	int historyIndex = command_history_length;
	int bufferLength = 0;
	int bufferIndex = 0;

	while (1){

		char key = getch();

		// special key (Arrow keys etc...)
		if (key == '\033'){
			char a = getch();     // this is just the [ character
			char b = getch();     // this is the actual data

			/*
				Up / Down Arrow
			*/
			if (b == 'A' || b == 'B'){
				if (bufferIndex > 0){
					printf("\033[%iD", bufferIndex);
				}

				// Up arrow
				if (b == 'A'){
					if (command_history_length == 0){
						continue;
					}

					// Save buffer so it can be restored
					if (historyIndex == command_history_length){
						memset(bufferStore, 0, strlen(bufferStore));
						strncpy(bufferStore, buffer, buffer_size);
					}

					if (historyIndex > 0){
						historyIndex --;
					}
					strcpy(buffer, command_history[historyIndex]);

				// Down arrow
				} else {
					if (historyIndex < command_history_length - 1){
						historyIndex ++;
						strcpy(buffer, command_history[historyIndex]);

					// Restore the buffer
					} else {
						historyIndex = command_history_length;
						memset(buffer, 0, strlen(buffer));
					  	strcpy(buffer, bufferStore);
					}
				}

				printf("%s ", buffer);

				// Figure out the size difference between the old
				// and new command in the buffer and blank it out
				if (strlen(buffer) < bufferLength){
					int difference = bufferLength - strlen(buffer);
					for (int i = 0; i < difference; i++){
						printf(" ");
					}
					printf("\033[%iD", difference);
				}

				// Update input values
				bufferLength = strlen(buffer);
				bufferIndex = bufferLength;

				printf("\033[%iD\033[%iC", bufferLength, bufferIndex - 1);

			/*
				Left Arrow
			*/
			} else if (b == 'C'){
				if (bufferIndex < buffer_size){
					bufferIndex ++;
					printf("\033[C");
				}

			/*
				Right Arrow
			*/
			} else if (b == 'D'){
				if (bufferIndex > 0){
					bufferIndex --;
					printf("\033[D");
				}
			}

		// return key
		} else if (key == 10){
			newEntry(buffer);

			for (int i = 0; i < command_history_length; i++){
				// printf("\033[%i;60H", i + 1);
				// printf("%i : %s       ", i, command_history[i]);
			}

			printf("\n");
			return buffer;
		
		// backspace key
		} else if (key == 27 || key == 127){
			if (bufferIndex > 0){
				if (bufferIndex - bufferLength){
					int copySize = (bufferLength - bufferIndex);

					char* after = malloc(sizeof(char) * copySize);
					strncpy(after, buffer+bufferIndex, copySize);
					strncpy(buffer+bufferIndex-1, after, copySize);

					buffer[bufferLength - 1] = '\0';

					// TODO: Convert this into one big print
					printf("\033[%iD", bufferIndex); // move cursor back
					printf("%s ", buffer);           // print buffer
					printf("\033[%iD", bufferLength);
					printf("\033[%iC", bufferIndex - 1);

					bufferIndex --;
					bufferLength --;

					free(after);

				} else {
					buffer[bufferIndex - 1] = '\0';
					bufferLength --;
					bufferIndex --;
					printf("\033[D \033[D");
				}
			}

		} else {
			if (bufferLength < buffer_size){
				
				if (bufferIndex < bufferLength){
					int copySize = (bufferLength - bufferIndex);

					char* after = malloc(sizeof(char) * copySize);
					strncpy(after, buffer+bufferIndex, copySize);

					buffer[bufferIndex] = key;
					strncpy(buffer+bufferIndex+1, after, copySize);

					// TODO: Convert this into one big print
					printf("\033[%iD", bufferIndex); // move cursor back
					printf("%s", buffer);            // print buffer
					printf("\033[%iD", bufferLength + 1);
					printf("\033[%iC", bufferIndex + 1);

					bufferIndex ++;
					bufferLength ++;

					printf("\033]0;%i\07", bufferIndex);

					free(after);

				} else {
					printf("%c", key);
					buffer[bufferIndex] = key;
					bufferLength ++;
					bufferIndex ++;
				}
			}
		}

	}

	return buffer;
}

static struct termios old, current;

void initTermios(int echo)
{
	tcgetattr(0, &old);
	current = old;
	current.c_lflag &= ~ICANON;
	if (echo) {
		current.c_lflag |= ECHO;
	} else {
		current.c_lflag &= ~ECHO;
	}
	tcsetattr(0, TCSANOW, &current);
}

void resetTermios(void)
{
	tcsetattr(0, TCSANOW, &old);
}

char getch_(int echo)
{
	char ch;
	initTermios(echo);
	ch = getchar();
	resetTermios();
	return ch;
}

char getch(void)
{
	return getch_(0);
}
