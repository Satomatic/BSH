#include <exec.h>

#include <stdio.h>
#include <signal.h>
#include <sys/wait.h>

void Shell::exec(args_t args){
    /**
     *  Up until this point we have been using the very
     *  convinient std::vector to store our arguments.
     *  But now we need to convert them to a regular old
     *  const char** for execvp.
     */
    std::vector<char*> cstrings;
    cstrings.reserve(args.size());

    for(size_t i = 0; i < args.size(); ++i)
        cstrings.push_back(const_cast<char*>(args[i].c_str()));

    /**
     *  Now we've done that we can mkae a fork and execute
     *  the command with arguments.
     */
	pid_t pid, wpid;
	int status;

	pid = fork();

	if (pid == 0){
		// enable signals for the fork process
		signal(SIGINT, SIG_DFL);

		if (execvp(args[0].c_str(), &cstrings[0]) == -1) {
			perror("bsh");
		}
		exit(EXIT_FAILURE);

	} else if (pid < 0) {
		perror("bsh");

	} else {
		do {
		  	wpid = waitpid(pid, &status, WUNTRACED);
		} while (!WIFEXITED(status) && !WIFSIGNALED(status));
	}
}
