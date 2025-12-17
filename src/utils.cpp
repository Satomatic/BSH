#include <utils.h>
#include <regex>

/**
 *  Split string into vector of smaller strings
 *
 *  @param std::string Data to split
 *  @param std::string String to split at
 *
 *  @return std::vector <std::string> Return vector
 */
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

/**
 *  Format string given a list of other strings
 *
 *  @param std::string String to format
 *  @param std::vector <std::string> List of sub strings
 *
 *  @return std::string Formatted string
 */
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

/**
 *  Remove whitespace from lead and tail of string
 *
 *  @param std::string String to strip
 *  @return std::string Stripped string
 */
std::string Utils::StripLeadTail(std::string text){
    /**
     *  @todo: This could definitely be one expression but I suck
     *         at regex so this will do for now.
     */
    text = std::regex_replace(text, std::regex("^ +"), "");
    text = std::regex_replace(text, std::regex(" +$"), "");
    return text;
}
