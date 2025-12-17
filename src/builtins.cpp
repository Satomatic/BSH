#include <builtins.h>
#include <iostream>
#include <unistd.h>
#include <global.h>

std::vector <builtin_t> Shell::builtins = {
    { "cd",      Shell::builtin_cd      },
    { "help",    Shell::builtin_help    },
    { "exit",    Shell::builtin_exit    },
    { "test",    Shell::builtin_test    },
    { "history", Shell::builtin_history }
};

std::vector <help_definition_t> Shell::builtin_defs = {
    { "cd  ", "Change directory" },
    { "help", "Shows help menu"  },
    { "exit", "Exits the shell"  }
};

/**
 *  Builtin change directory command
 */
int Shell::builtin_cd(args_t args){
    if (args.size() > 1){
        chdir(args[1].c_str());
        return 0;
    }

    return -1;
}

/**
 *  Builtin help command
 */
int Shell::builtin_help(args_t args){
    /**
     *  So this is actually really cringe and shouldn't just be loads
     *  of printf calls. It really should be dynamically generated from
     *  the builtin list, but for now, this works just fine for a basic
     *  help command.
     */
    
    printf("\033[1mBSH (Brian's Shell)\033[0m\n");
    printf("Version 0.1.0\n\n");

	printf("\033[1mBuilt in commands\033[0m\n");

    for (int i = 0; i < Shell::builtin_defs.size(); i++)
        printf("  %s | %s\n",
            Shell::builtin_defs[i].command.c_str(),
            Shell::builtin_defs[i].description.c_str()
        );

    return 0;
}

int Shell::builtin_history(args_t args){
    for (int i = 0; i < args.size(); i++){
        if (args[i] == "clear" || args[i] == "-c"){
            Shell::CommandHistory.clear();
            return 0;
        }
    }

    for (int i = 0; i < Shell::CommandHistory.size(); i++)
        printf("   %d  %s\n", i, Shell::CommandHistory[i].c_str());
    
    return 0;
}

/**
 *  Builtin exit command
 */
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
