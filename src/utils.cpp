#include <utils.h>

std::vector <std::string> Utils::SplitString(std::string data, std::string split){
    std::vector <std::string> returnVector = {};
    std::string cdata = "";

    for (int i = 0; i < data.size(); i++){
        if (i + split.size() <= data.size() && data.substr(i, split.size()) == split){
            returnVector.push_back(cdata);
            cdata = "";
            i += split.size() - 1;
            continue;
        }

        cdata += data[i];
    }

    if (cdata.size() > 0)
        returnVector.push_back(cdata);

    return returnVector;
}

std::string Utils::format(std::string text, std::vector <std::string> fmt_list){
    int fmt_index = 0;
    std::string returnString = "";
    for (int i = 0; i < text.size(); i++){
        if (text[i] == '$'){
            returnString += fmt_list[fmt_index];
            fmt_index++;
            continue;
        }

        returnString += text[i];
    }

    return returnString;
}
