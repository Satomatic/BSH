#include <data/arguments.h>
#include <string.h>

bool Shell::DebugMode = false;

/**
 *  This function will process the arguments passed to the
 *  shell when it is ran.
 */
void Shell::ProcessArguments(char** argv, int argc){
    for (int i = 1; i < argc; i++){
        if (strcmp(argv[i], "--debug") == 0){
            Shell::DebugMode = true;
        }
    }
}