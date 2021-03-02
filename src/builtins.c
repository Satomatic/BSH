#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include "include/builtins.h"

char *builtins_str[] = {
	"cd",
	"help",
	"exit",
	"test"
};

void (*builtins_func[]) (char**) = {
	&bssh_builtin_cd,
	&bssh_builtin_help,
	&bssh_builtin_exit,
	&bssh_builtin_test
};

int builtinlen() {
  return sizeof(builtins_str) / sizeof(char *);
}

/*
	Built in function definitions
*/
void bssh_builtin_cd(char** args){
	if (args[1] != NULL)
		chdir(args[1]);
}

void bssh_builtin_help(char** args){
	printf("%sBSH (Brian's Shell)%s", "\033[1m", "\033[0m\n");
	printf("Version 0.1.0\n\n");

	printf("%sBuilt in commands%s", "\033[1m", "\033[0m\n");
	printf("  cd   | Change directory\n");
	printf("  help | Shows help info\n");
	printf("  exit | Exits the shell\n\n");
}

void bssh_builtin_exit(char** args){
	exit(0);
}

void bssh_builtin_test(char** args){
	printf("%s:\n", args[0]);

	int i = 1;
	while (args[i]){
		printf("  %s\n", args[i]);
		i++;
	}
}