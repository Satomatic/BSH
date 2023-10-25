#include <serror.h>

void Shell::HandleError(int err, args_t args){
    printf("\033[1m'%s' returned with error code %d\033[0m\n", args[0].c_str(), err);
}