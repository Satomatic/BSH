#ifndef _BSH_BUILTINS_H_
#define _BSH_BUILTINS_H_

extern char *builtins_str[];
extern void (*builtins_func[]) (char**);

int builtinlen();
void bssh_builtin_cd(char** args);
void bssh_builtin_help(char** args);
void bssh_builtin_exit(char** args);
void bssh_builtin_test(char** args);

#endif