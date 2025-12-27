#include <input.h>
#include <history.h>
#include <global.h>

#include <termios.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <stdio.h>

char getch(void);

std::string Shell::GetInput(std::string prompt, int limit) {
	std::string returnString = "";
	bool takingInput = true;

    std::string commandSave = "";

	while (takingInput) {
		/**
		 *  For each input we take we need to print it to the screen.
		 *  By using a carrage return we can do it pretty easily and
		 *  it won't spring to many issues, although, this is by far
		 *  not the best way to do it.
		 */
		printf("\r%s%s \b",
			prompt.c_str(),
			returnString.c_str()
		);

		char key = getch();

        /**
         *  Processing for arrow keys
         */
        if (key == '\033'){
            getch();
            char direction = getch();

            switch (direction){
                // Up arrow
                case 'A':
                    printf("\r%s", prompt.c_str());
                    for (int i = 0; i < returnString.size() + 5; i++)
                        printf(" ");

                    if (Shell::HistoryLength == 0)
                        break;

                    if (Shell::HistoryLength == Shell::HistoryIndex)
                        commandSave = returnString;

                    if (Shell::HistoryIndex > 0)
                        Shell::HistoryIndex --;

                    returnString = Shell::CommandHistory[Shell::HistoryIndex];
                    break;

                // Down arrow
                case 'B':
                    printf("\r%s", prompt.c_str());
                    for (int i = 0; i < returnString.size() + 5; i++)
                        printf(" ");

                    if (Shell::HistoryIndex < Shell::HistoryLength - 1){
                        Shell::HistoryIndex ++;
                        returnString = Shell::CommandHistory[Shell::HistoryIndex];
                    
                    } else {
                        Shell::HistoryIndex = Shell::HistoryLength;
                        returnString = commandSave;
                    }

                    break;

                // Left Arrow
                case 'D':
                    break;

                // Right arrow
                case 'C':
                    break;
            }

            continue;
        }

		/**
		 *  Using a switch might break a little when we get into special
		 *  key sequences such as arrows, but for now this is good and
		 *  we will cross that bridge when it comes.
		 */
		switch (key) {
            // Return key
            case '\n':
				takingInput = false;
				break;

            // Backspace key
	    		case 27:
			case 127:
			case '\b':
				if (returnString.size() > 0)
					returnString = returnString.substr(0, returnString.size() - 1);
				break;

            // Any other key
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
