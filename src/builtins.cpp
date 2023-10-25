#include <builtins.h>
#include <iostream>

std::vector <builtin_t> Shell::builtins = {
    { "cd",   Shell::builtin_cd   },
    { "help", Shell::builtin_help },
    { "exit", Shell::builtin_exit },
    { "test", Shell::builtin_test }
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

int Shell::builtin_test(args_t args){
    std::cout << "\033[1m" << args[0] << "\033[0m" << std::endl;
    for (int i = 1; i < args.size(); i++){
        std::cout << " - " << args[i] << std::endl;
    }

    return 0;
}