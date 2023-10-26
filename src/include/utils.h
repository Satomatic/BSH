#ifndef __BSH_UTILS_HEADER__
#define __BSH_UTILS_HEADER__

#include <string>
#include <vector>

namespace Utils {
    std::vector <std::string> SplitString(std::string, std::string);
    std::string format(std::string, std::vector <std::string>);
    std::string StripLeadTail(std::string);
};

#endif