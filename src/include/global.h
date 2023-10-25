#ifndef __BSH_GLOBAL_HEADER__
#define __BSH_GLOBAL_HEADER__

#include <vector>
#include <string>

typedef std::vector <std::string> args_t;

namespace Shell {
	extern std::vector <std::string> CommandHistory;
	extern std::string PromptTemplate;
	extern std::string PromptData;
	extern bool Open;
};

#endif