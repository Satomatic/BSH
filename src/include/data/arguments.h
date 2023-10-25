#ifndef __BSH_DATA_ARGUMENTS_HEADER__
#define __BSH_DATA_ARGUMENTS_HEADER__

#include <global.h>

namespace Shell {
    extern bool DebugMode;

    void ProcessArguments(char**, int);
};

#endif