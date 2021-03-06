#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "include/utils.h"
#include "include/auto.h"

void autoComplete(char* buffer){
	/*
		Get the directory size from buffer
	*/
	int start = 0;
	int end = 0;
	int i = 0;
	while (buffer[i] != '\0'){
		if (buffer[i] == ' '){
			start = i + 1;
		} else if (buffer[i] == '/'){
			end = i + 1;
		}

		i++;
	}

	char* buff = buffer + start;
	int dirSize = end - start;

	if (end == 0){
		dirSize = 0;
	}

	/*
		Create string with directory name
	*/
	char* directory = malloc(dirSize);
	memset(directory, 0, dirSize);
	strncpy(directory, buff, dirSize);

	/*
		Create a string with filename
	*/
	char* filename = malloc(100);
	memset(filename, 0, 100);
	strncpy(filename, buff+dirSize, 10);

	/*
		If directory is not specified,
		assume current
	*/
	if (end == 0){
		directory = "./";
	}

	char** listing = listdir(directory);
	int index = 0;

	if (strlen(filename) == 0){
		index = 2;
	} else {
		i = 2;
		while (listing[i] != NULL){
			if (strlen(listing[i]) >= strlen(filename)){
				if (strncmp(listing[i], filename, strlen(filename)) == 0){
					index = i;
					break;
				}
			}

			i++;
		}
	}

	/*
		Insert into buffer
	*/
	if (index != 0){
		strcpy(buff+dirSize, listing[index]);
	}
}
