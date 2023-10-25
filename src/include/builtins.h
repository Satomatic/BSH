#ifndef __BSH_BUILTINS_HEADER__
#define __BSH_BUILTINS_HEADER__

#include <global.h>
#include <functional>

typedef struct {
    std::string command;
    std::function <int(args_t)> callback;
} builtin_t;

namespace Shell {
    int builtin_cd(args_t);
    int builtin_help(args_t);
    int builtin_exit(args_t);
    int builtin_test(args_t);

    extern std::vector <builtin_t> builtins;
}

#endif