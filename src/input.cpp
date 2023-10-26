#include <input.h>

#include <termios.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <stdio.h>

char getch(void);

std::string Shell::GetInput(std::string prompt, int limit) {
	std::string returnString = "";
	bool takingInput = true;

	while (takingInput) {
		/**
		 *  For each input we take we need to print it to the screen.
		 *  By using a carrage return we can do it pretty easily and
		 *  it won't spring to many issues, although, this is by far
		 *  not the best way to do it.
		 */
		printf("\r%s %s \b",
			prompt.c_str(),
			returnString.c_str()
		);

		/**
		 *  @todo: I would quite like to make this code work on windows
		 *         though it does take input very differently so that
		 *         will be a ball-ache.
		 */
		char key = getch();

		/**
		 *  Using a switch might break a little when we get into special
		 *  key sequences such as arrows, but for now this is good and
		 *  we will cross that bridge when it comes.
		 */
		switch (key) {
			case GETCH_RETURN:
				takingInput = false;
				break;

			/**
			 *  @todo: This will eventually need to take into account the
			 *         cursor position once arrow keys are sorted.
			 */
			case GETCH_BACKSAPCE:
				if (returnString.size() > 0)
					returnString = returnString.substr(0, returnString.size() - 1);
				break;

			case GETCH_BACKSPACE_ALT:
				if (returnString.size() > 0)
					returnString = returnString.substr(0, returnString.size() - 1);
				break;

			default:
				if (isascii(key))
					returnString += key;
				break;
		}
	}

	return returnString;
}

static struct termios old, current;

void initTermios(int echo)
{
	tcgetattr(0, &old);
	current = old;
	current.c_lflag &= ~ICANON;
	if (echo) {
		current.c_lflag |= ECHO;
	}
	else {
		current.c_lflag &= ~ECHO;
	}
	tcsetattr(0, TCSANOW, &current);
}

void resetTermios(void)
{
	tcsetattr(0, TCSANOW, &old);
}

char getch_(int echo)
{
	char ch;
	initTermios(echo);
	ch = getchar();
	resetTermios();
	return ch;
}

char getch(void)
{
	return getch_(0);
}