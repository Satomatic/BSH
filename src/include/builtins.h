#ifndef _BSSH_BUILTINS_H
#define _BSSH_BUILTINS_H

extern char *builtins_str[];
extern void (*builtins_func[]) (char**);

int builtinlen();
void bssh_builtin_cd(char** args);
void bssh_builtin_help(char** args);
void bssh_builtin_exit(char** args);

#endif