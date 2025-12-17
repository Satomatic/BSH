#include <history.h>
#include <global.h>

#include <fstream>

int Shell::HistoryLength = 0;
int Shell::HistoryIndex = 0;

std::string Shell::HistoryFile = "~/.bshhistory";

void Shell::InitHistory(){
    /**
     *  Generate the user specific history file path
     */
    std::string formatted = "";

    for (int i = 0; i < Shell::HistoryFile.size(); i++){
        if (Shell::HistoryFile[i] == '~'){
            formatted += "/home/";
            formatted += getenv("USER");
            continue;
        }
        formatted += Shell::HistoryFile[i];
    }

    Shell::HistoryFile = formatted;

    /**
     *  Load the previous history from the file.
     */
    std::ifstream fileRead(Shell::HistoryFile.c_str());
    std::string cline = "";

    while (getline(fileRead, cline)){
        Shell::HistoryInsert(cline);
    }

    fileRead.close();
}

void Shell::HistoryInsert(std::string command){
    Shell::CommandHistory.push_back(command);
    Shell::HistoryLength ++;
    Shell::HistoryIndex ++;
}

void Shell::SaveHistory(){
    std::ofstream writeFile(Shell::HistoryFile.c_str());

    for (int i = 0; i < Shell::CommandHistory.size(); i++){
        writeFile << Shell::CommandHistory[i] << std::endl;
    }

    writeFile.close();
}
