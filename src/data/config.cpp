#include <sys/stat.h>
#include <data/config.h>
#include <parser.h>
#include <utils.h>
#include <fstream>

std::string Shell::ConfigPath = "~/.bshrc";
std::vector <config_t> Shell::ShellConfig = {
    { "prompt_template",  "( $user ) [ $cwd $git ] ~$ " },
    { "foreground_color", "0"   },
    { "background_color", "0"   },
    { "user_color",       "80"  },
    { "cwd_color",        "177" },
    { "git_color",        "179" }
};

void Shell::InitConfig(){
    /**
     *  Firstly we need to format the config file path
     *  to have the current user name.
     */
    std::string formattedPath = "";
    for (int i = 0; i < Shell::ConfigPath.size(); i++){
        if (Shell::ConfigPath[i] == '~'){
            formattedPath += "/home/";
            formattedPath += getenv("USER");
            continue;
        }
        formattedPath += Shell::ConfigPath[i];
    }

    /**
     *  Check if the config file exists, if it doesn't
     *  then we can just write the default values into
     *  a new file.
     */
    struct stat buffer;
    int exists = (stat (formattedPath.c_str(), &buffer) == 0); 

    if (!exists){
        std::ofstream fileWrite(formattedPath);

        for (int i = 0; i < Shell::ShellConfig.size(); i++)
            fileWrite << Shell::ShellConfig[i].key << "=" << Shell::ShellConfig[i].value << std::endl;

        fileWrite.close();
        return;
    }

    /**
     *  If the config file does exist then we can load
     *  the values into the config vector.
     */

    std::ifstream fileRead(formattedPath);
    std::string cline = "";

    while (getline(fileRead, cline)){
        std::vector <std::string> split = Utils::SplitString(cline, "=");
        if (split.size() == 1) split.push_back("");
        
        /**
         *  If the key exists within the config vector
         *  then we can just set the value.
         */
        bool f = false;
        for (int i = 0; i < Shell::ShellConfig.size() && !f; i++){
            if (Shell::ShellConfig[i].key == split[0]){
                Shell::ShellConfig[i].value = split[1];
                f = true;
            }
        }
        
        /**
         *  Otherwise we can just push_back a new entry
         *  with a new key and value.
         * 
         *  This should never really happen, if it does
         *  then theres a bad key somewhere.
         */
        if (f == true)
            continue;
        
        Shell::ShellConfig.push_back(
            {split[0], split[1]}
        );
    }

    fileRead.close();
}

std::string Shell::GetConfigValue(std::string key){
    for (int i = 0; i < Shell::ShellConfig.size(); i++)
        if (Shell::ShellConfig[i].key == key)
            return Shell::ShellConfig[i].value;

    return "";
}
