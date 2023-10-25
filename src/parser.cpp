#include <parser.h>
#include <data/config.h>
#include <iostream>
#include <unistd.h>
#include <regex>

std::vector <std::string> SplitString(std::string data, std::string split){
    std::vector <std::string> returnVector = {};
    std::string cdata = "";

    for (int i = 0; i < data.size(); i++){
        if (i + split.size() <= data.size() && data.substr(i, split.size()) == split){
            returnVector.push_back(cdata);
            cdata = "";
            i += split.size() - 1;
            continue;
        }

        cdata += data[i];
    }

    if (cdata.size() > 0)
        returnVector.push_back(cdata);

    return returnVector;
}

std::string format(std::string text, std::vector <std::string> fmt_list){
    int fmt_index = 0;
    std::string returnString = "";
    for (int i = 0; i < text.size(); i++){
        if (text[i] == '$'){
            returnString += fmt_list[fmt_index];
            fmt_index++;
            continue;
        }

        returnString += text[i];
    }

    return returnString;
}

#define PROMPT_KEY_REPLACER "\033[38;5;$m$\033[0m"

std::string Shell::ParsePrompt(std::string data){
    std::string pc_template = "\033[38;5;$m$\033[0m";

    std::string name_replacer = format(PROMPT_KEY_REPLACER, {
        Shell::ShellConfig[3].value,
        getenv("USER")
    });

    std::string dirc_replacer = format(PROMPT_KEY_REPLACER, {
        Shell::ShellConfig[4].value,
        getcwd(NULL, 0)
    });

    data = std::regex_replace(data, std::regex("\\$user"), name_replacer);
    data = std::regex_replace(data, std::regex("\\$cwd"), dirc_replacer);

    std::string returnString = format("\033[$m\033[$m", {
        Shell::ShellConfig[1].value,
        Shell::ShellConfig[2].value
    });

    returnString += data + "\033[0m";

    return returnString;
}