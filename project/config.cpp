#include "config.h"

Config::Config()
    : isShowKbvChecked(false)
    , isShowPadValueChecked(false)
    , xInputIndex(0)
{
}

constexpr size_t Config::GetConfigSize()
{
    return sizeof(Config);
}

void Config::ReadConfig()
{
    std::ifstream ifs(fileName, std::ios_base::binary);
    if (!ifs.is_open())
        return;
    static char buffer[GetConfigSize()];
    size_t size = ifs.read(buffer, GetConfigSize()).gcount();
    if (size == 0)
        return;
    *this = *reinterpret_cast<Config*>(buffer);
}

void Config::SaveConfig()
{
    std::ofstream ofs(fileName, std::ios_base::binary | std::ios_base::trunc);
    if (!ofs.is_open())
        return;
    ofs.write(
        reinterpret_cast<char*>(this),
        GetConfigSize());
}
