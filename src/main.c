#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "include/builtins.h"
#include "include/global.h"
#include "include/parse.h"
#include "include/utils.h"

char promptTemplate[] = "( \033[38;5;80m%s\033[0m ) [ \033[38;5;177m%s\033[0m ] ~$ ";

void exec(char** args){
	pid_t pid, wpid;
	int status;

	pid = fork();

	if (pid == 0){
		// enable signals for the fork process
		signal(SIGINT, SIG_DFL);

		if (execvp(args[0], args) == -1) {
			perror("bsh");
		}
		exit(EXIT_FAILURE);

	} else if (pid < 0){
		perror("bsh");

	} else {
		do {
		  	wpid = waitpid(pid, &status, WUNTRACED);
		} while (!WIFEXITED(status) && !WIFSIGNALED(status));
	}
}

char** command_history;
int command_history_length = 0;
int command_history_max = 100;
int debug = 0;

int main (int argc, char** argv){
	if (strcmp(argv[0], "./a.out") == 0){
		debug = 1;
	}

	// disable signals for the main process
	signal(SIGINT, SIG_IGN);
	signal(SIGTSTP, SIG_IGN);

	// allocate command history
	command_history = malloc(sizeof(char*) * command_history_max);

	while (1){
		/*
			Display prompt
		*/
		if (debug == 1){
			printf("( \033[38;5;42m%s\033[0m ) ", "debug");
		}

		const char* user = getenv("USER");
		const char* cdir = getcwd(NULL,0);
		printf(promptTemplate, user, cdir);

		/*
			Take in user input and split at '&'
		*/
		char* input = getInput(512);
		char** commands = splitCommand(input);
		
		/*
			For every command
		*/
		for (int i = 0; commands[i] != NULL; i++){
			char** array = splitInput(commands[i]);

			if (array[0] == NULL) continue;

			/*
				Check for build in functions
			*/
			int found = 0;
			for (int i = 0; i < builtinlen(); i++){
				if (strcmp(array[0], builtins_str[i]) == 0){
					(*builtins_func[i])(array);
					found = 1;
					break;
				}
			}

			/*
				If no builtins were found, execute
			*/
			if (found == 0){
				exec(array);
			}
		
			free(array);
		}

		free(input);
	}

	free(command_history);

	return 0;

}
