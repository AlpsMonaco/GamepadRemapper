#ifndef CONFIG_H
#define CONFIG_H

#include <fstream>

struct Config
{
    bool isShowKbvChecked;
    bool isShowPadValueChecked;
    unsigned long xInputIndex;

    Config();
    ~Config() = default;

    static constexpr char fileName[] = "config.bin";
    void ReadConfig();
    void SaveConfig();
    static constexpr size_t GetConfigSize();
};

#endif // CONFIG_H
