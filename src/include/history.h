#ifndef __BSH_HISTORY_HEADER__
#define __BSH_HISTORY_HEADER__

#include <string>

namespace Shell {
    void HistoryInsert(std::string);
    void SaveHistory(std::string);

    extern int HistoryLength;
    extern int HistoryIndex;
}

#endif
