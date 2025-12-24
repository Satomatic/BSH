#include <git.h>

#include <iostream>

/**
 *  Get the repo name if we are in a git work tree.
 *
 *  @note: This only works if git is installed on the system, we
 *         could make a more complex function that would work
 *         without git, but for now this works.
 *
 *         This may also break cross platform functionality but
 *         not to a horrible degree.
 */
std::string Git::GetRepoString(){
    char buffer[128];
    std::string result = "";

    FILE* p = popen("git branch 2>&1", "r");

    while (fgets(buffer, 128, p) != NULL)
        result += buffer;

    int err = pclose(p);

    result = result.substr(0, result.size() - 1);

    if (err != 0)
        return "\b";

    return result;
}
