#ifndef __BSH_BUILTINS_HEADER__
#define __BSH_BUILTINS_HEADER__

#include <global.h>
#include <functional>

typedef struct {
    std::string command;
    std::function <int(args_t)> callback;
} builtin_t;

typedef struct {
    std::string command;
    std::string description;
} help_definition_t;

namespace Shell {
    int builtin_cd(args_t);
    int builtin_help(args_t);
    int builtin_exit(args_t);
    int builtin_test(args_t);
    int builtin_history(args_t);

    extern std::vector <builtin_t> builtins;
    extern std::vector <help_definition_t> builtin_defs;
}

#endif
