#include <global.h>
#include <input.h>
#include <parser.h>
#include <builtins.h>
#include <serror.h>
#include <exec.h>
#include <data/config.h>
#include <utils.h>
#include <iostream>
#include <string.h>

std::vector <std::string> Shell::CommandHistory = {};
bool Shell::Open = true;

int main(int argc, char** argv) {
	Shell::InitConfig();

	std::string PromptTemplate = Shell::GetConfigValue("prompt_template");

	/**
	 *  @todo: Disable default signals
	 */

	while (Shell::Open) {
		std::string input = Shell::GetInput(Shell::ParsePrompt(PromptTemplate), 248);

		/**
		 *  Very simple split operation to seperate multiple command
		 *  joint with '&&'. Currently this wont take into account
		 *  escaping characters, but for now it works.
		 */
		std::vector <std::string> commandSplit = Shell::ParseCommandList(input);
		
		std::cout << '\n';

		/**
		 *  For each command, we need to seperate it into indevidual
		 *  arguments.
		 * 
		 *  We will also remove any whitespace surrounding the commands
		 *  to avoid any issues.
		 * 
		 *  Just for testing I'm going to use a simple string split, then
		 *  once proof of concept is finished we can make a proper parser.
		 */
		for (int i = 0; i < commandSplit.size(); i++){
			// @todo: Remove leading and trailing white space
			args_t arguments = Shell::ParseArgumentList(commandSplit[i]);

			if (arguments.size() == 0) continue;

			bool f = false;
			for (int b = 0; b < Shell::builtins.size() && !f; b++){
				if (Shell::builtins[b].command == arguments[0]){
					int err = Shell::builtins[b].callback(arguments);
					if (err < 0) Shell::HandleError(err, arguments);
					f = true;
				}
			}

			if (!f){
				Shell::exec(arguments);
			}
		}
	}
}