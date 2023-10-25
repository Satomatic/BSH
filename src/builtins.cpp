#include <builtins.h>

std::vector <builtin_t> Shell::builtins = {
    { "cd",   Shell::builtin_cd   },
    { "help", Shell::builtin_help },
    { "exit", Shell::builtin_exit }
};

int Shell::builtin_cd(args_t args){
    return -1;
}

int Shell::builtin_help(args_t args){
    return 0;
}

int Shell::builtin_exit(args_t args){
    if (args.size() >= 2)
        exit(atoi(args[1].c_str()));

    exit(0);
}