#ifndef __BSH_CONFIG_HEADER__
#define __BSH_CONFIG_HEADER__

#include <string>
#include <vector>

typedef struct {
    std::string key;
    std::string value;
} config_t;

namespace Shell {
    extern std::string ConfigPath;
    extern std::vector <config_t> ShellConfig;

    void InitConfig();

    std::string GetConfigValue(std::string);
    void SetConfigValue(std::string, std::string);
};

#endif