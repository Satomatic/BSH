#ifndef __BSH_PARSER_HEADER__
#define __BSH_PARSER_HEADER__

#include <global.h>

namespace Shell {
    std::vector <std::string> ParseCommandList(std::string);
    args_t ParseArgumentList(std::string);
    std::string ParsePrompt(std::string);
};

std::vector <std::string> SplitString(std::string, std::string);

#endif