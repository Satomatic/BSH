#include <parser.h>
#include <data/config.h>
#include <iostream>
#include <unistd.h>
#include <regex>
#include <utils.h>

#define PROMPT_KEY_REPLACER "\033[38;5;$m$\033[0m"

/**
 *  This will take the prompt template found in the
 *  config file and replace it's keywords with the
 *  relevant values and insert the colour sequences
 *  which are also defined in the config file.
*/
std::string Shell::ParsePrompt(std::string data){
    std::string pc_template = "\033[38;5;$m$\033[0m";

    std::string name_replacer = Utils::format(PROMPT_KEY_REPLACER, {
        Shell::ShellConfig[3].value,
        getenv("USER")
    });

    std::string dirc_replacer = Utils::format(PROMPT_KEY_REPLACER, {
        Shell::ShellConfig[4].value,
        getcwd(NULL, 0)
    });

    data = std::regex_replace(data, std::regex("\\$user"), name_replacer);
    data = std::regex_replace(data, std::regex("\\$cwd"), dirc_replacer);

    std::string returnString = Utils::format("\033[$m\033[$m", {
        Shell::ShellConfig[1].value,
        Shell::ShellConfig[2].value
    });

    returnString += data + "\033[0m";

    return returnString;
}

/**
 *  Split the full input into multiple commands.
 * 
 *  For example: `cd build && make`
 *  Will be split into the two commands.
 * 
 *  This will account for escape characters and
 *  strings.
 */
args_t Shell::ParseCommandList(std::string data){
    /**
     *  We will keep track of if we are inside a "string"
     *  or a 'character' as we don't want to interrupt that
     *  data.
     */
    bool stringMode = false;
    bool charMode = false;

    args_t returnVector = {};

    std::string currentValue = "";

    for (int i = 0; i < data.size(); i++){
        /**
         *  If the previous character is '\' then we
         *  don't need to apply any rules to the current
         *  character and should treat it as literal.
         */
        bool escapePre = (i >= 1 && data[i - 1] == '\\');
        
        /**
         *  Assuming the previous character isn't '\' then
         *  we need to enter / escape stringMode and charMode
         *  when we reach their relevent characters.
         */
        if (data[i] == '"' && !escapePre) stringMode = !stringMode;
        if (data[i] == '\'' && !escapePre) charMode = !charMode;

        /**
         *  Assuming we aren't in a string, character string, and
         *  the previous character doesn't escape. If we have
         *  reached '&&' then we need to split.
         */
        if (i + 2 <= data.size() && data.substr(i, 2) == "&&" &&
            !stringMode && !charMode && !escapePre
        ){
            returnVector.push_back(currentValue);
            currentValue = "";

            /**
             *  It is also important that we skip past the next
             *  character so it doesn't get picked up and added
             *  into the next command.
             */
            i ++;

            continue;
        }

        currentValue += data[i];
    }

    /**
     *  If there are any value left in 'currentValue' then we
     *  can go ahead and push that to the return vector.
     */
    if (currentValue.size() > 0)
        returnVector.push_back(currentValue);

    return returnVector;
}

/**
 *  This function will take a command and split it up into
 *  a list of arguments to get executed.
 *  
 *  This will account for escape sequences, "strings" and
 *  'character strings'
 */
args_t Shell::ParseArgumentList(std::string data){
    /**
     *  Just like in ParseCommandList, we use these two booleans
     *  to keep track of whether we are inside a string or not.
    */
    bool stringMode = false;
    bool charMode = false;

    args_t returnVector = {};
    std::string currentValue = "";

    for (int i = 0; i < data.size(); i++){
        bool escapePre = (i >= 1 && data[i - 1] == '\\');
    
        if (data[i] == '"' && !escapePre) stringMode = !stringMode;
        if (data[i] == '\'' && !escapePre) charMode = !charMode;

        if (data[i] == ' ' && !stringMode && !stringMode && !escapePre){
            returnVector.push_back(currentValue);
            currentValue = "";
            continue;
        }

        if (data[i] != '\\') currentValue += data[i];
    }

    if (currentValue.size() > 0)
        returnVector.push_back(currentValue);

    return returnVector;
}