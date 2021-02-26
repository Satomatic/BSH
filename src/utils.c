#include <termios.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
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

char* getInput(int buffer_size){
	char* buffer = malloc(sizeof(char) * buffer_size);
	int bufferLength = 0;
	int bufferIndex = 0;

	while (1){

		char key = getch();

		// special key (Arrow keys etc...)
		if (key == '\033'){
			char a = getch();     // this is just the [ character
			char b = getch();     // this is the actual data

			if (b == 'A'){        // up

			} else if (b == 'B'){ // down


			} else if (b == 'C'){ // right
				if (bufferIndex < buffer_size){
					bufferIndex ++;
					printf("\033[C");
				}


			} else if (b == 'D'){ // left
				if (bufferIndex > 0){
					bufferIndex --;
					printf("\033[D");
				}
			}

		// return key
		} else if (key == 10){
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
					buffer[bufferIndex] = '\0';
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