#ifndef __BSH_HISTORY_HEADER__
#define __BSH_HISTORY_HEADER__

#include <string>

namespace Shell {
    void InitHistory();
    void HistoryInsert(std::string);
    void SaveHistory();

    extern std::string HistoryFile;

    extern int HistoryLength;
    extern int HistoryIndex;
}

#endif
