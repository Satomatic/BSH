#include <serror.h>

#include <iostream>

void Shell::HandleError(int err, args_t args){
    printf("\033[1m'%s' returned with error code %d\033[0m\n", args[0].c_str(), err);
}

void Shell::DebugInput(args_t args){
    printf("\033[1mExecuting: \033[0m%s\n", args[0].c_str());
    for (int i = 1; i < args.size(); i++){
        printf("\033[1m  %d\033[0m : %s\n", i, args[i].c_str());
    }
}