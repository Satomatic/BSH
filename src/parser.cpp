#include <parser.h>

std::vector <std::string> SplitString(std::string data, std::string split){
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