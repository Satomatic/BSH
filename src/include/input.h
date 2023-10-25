#ifndef __BSH_INPUT_HEADER__
#define __BSH_INPUT_HEADER__

#include <string>

#define GETCH_RETURN 10
#define GETCH_BACKSAPCE 27
#define GETCH_BACKSPACE_ALT 127

namespace Shell {
	std::string GetInput(std::string, int);
};

#endif