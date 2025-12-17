#include <history.h>
#include <global.h>

int Shell::HistoryLength = 0;
int Shell::HistoryIndex = 0;

void Shell::HistoryInsert(std::string command){
    Shell::CommandHistory.push_back(command);
    Shell::HistoryLength ++;
    Shell::HistoryIndex ++;
}

void Shell::SaveHistory(std::string location){

}
