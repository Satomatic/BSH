#include <termios.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <stdio.h>

#include "include/history.h"
#include "include/global.h"
#include "include/utils.h"
#include "include/auto.h"

char** listdir(char* directory){
	/*
		Initial allocation of array
	*/
	char** array = malloc(sizeof(char*));
	int position = 0;

	DIR *dir;
	struct dirent *ent;

	if ((dir = opendir (directory)) != NULL) {
		while ((ent = readdir (dir)) != NULL) {
			
			/*
				Copy file name into a string
			*/
			char* item = malloc(strlen(ent->d_name) + 2);
			memset(item, 0, strlen(ent->d_name) + 2);
			strcpy(item, ent->d_name);

			if (ent->d_type == DT_DIR){
				item[strlen(ent->d_name)] = '/';
			}
			
			/*
				Reallocate array with extended size
			*/
			array = (char **) realloc(array, sizeof(char*) * (position + 2));
			
			/*
				free array if realloc fails
			*/
			if (array == NULL){
				free(array);
				return array;
			}
			
			/*
				Insert item
			*/
			array[position] = item;
			position ++;
		}
		closedir (dir);

	} else {
		perror ("bsh");
	}

	/*
		Last value should always be NULL
	*/
	array[position] = NULL;

	return array;
}

const char* getextension(const char* filename){
    const char *dot = strrchr(filename, '.');
    if(!dot || dot == filename) return "";
    return dot+1;
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
				Right Arrow
			*/
			} else if (b == 'C'){
				if (bufferIndex < bufferLength){
					bufferIndex ++;
					printf("\033[C");
				}

			/*
				Left Arrow
			*/
			} else if (b == 'D'){
				if (bufferIndex > 0){
					bufferIndex --;
					printf("\033[D");
				}
			}

		// tab key
		} else if (key == 9){
			if (bufferIndex > 0){
				autoComplete(buffer);
				
				if (bufferIndex > 0) printf("\033[%iD", bufferIndex);
				printf("%s ", buffer);
				printf("\033[%iD", bufferLength);

				if (bufferIndex - 1 > 0) printf("\033[%iC", bufferIndex - 1);
				
				bufferLength = strlen(buffer);
				bufferIndex = bufferLength;
			}

		// return key
		} else if (key == 10){
			buffer[bufferLength] = '\0';
			newEntry(buffer);

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

					/*
						Get everything after cursor
					*/
					char* after = malloc(sizeof(char) * copySize);
					strncpy(after, buffer+bufferIndex, copySize);

					/*
						Insert key and copy `after` into buffer
					*/
					buffer[bufferIndex] = key;
					strncpy(buffer+bufferIndex+1, after, copySize);

					/*
						If cursor isn't at the beginning of line
					*/
					if (bufferIndex > 0){
						printf("\033[%iD", bufferIndex);
					}

					printf("%s", buffer);
					printf("\033[%iD", bufferLength + 1);
					printf("\033[%iC", bufferIndex + 1);

					bufferIndex ++;
					bufferLength ++;

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
