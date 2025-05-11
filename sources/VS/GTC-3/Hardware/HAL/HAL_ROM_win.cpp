// 2025/05/11 11:36:21 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Hardware/HAL/HAL.h"
#include <fstream>


void HAL_ROM::SaveSettings(const Settings &set)
{
    std::ofstream file("settings.bin", std::ios::out | std::ios::binary);
    file.write(&set, sizeof(set));
}


bool HAL_ROM::LoadSettings(Settings &set)
{
    return false;
}
