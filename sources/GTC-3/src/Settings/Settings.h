// 2022/05/05 15:22:56 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Display/Display.h"


#define NUM_MEASURES_TO_CONTROL 4


struct TypeDisplayedInformation
{
    enum E
    {
        MeasureTemperature,
        MeasureHumidity,
        MeasurePressure,
        MeasureDewPoint,
        AllMeasures,
        Menu,
        Count
    };

    E value;

    TypeDisplayedInformation &operator++()
    {
        value = (E)(value + 1);

        return *this;
    }

    bool IsAllMeasures() const { return value == AllMeasures; }
};



struct SettingsDisplay
{
    uint8                       show_measure[Measure::Count];
    TypeDisplayedInformation    typeDisplaydInfo;
};


struct SettingsMeasures
{
    int limit_min[Measure::Count];
    int limit_max[Measure::Count];
    float value_min[Measure::Count];
    float value_max[Measure::Count];
};


struct SettingsSystem
{
    int deviceID;
};


struct Settings
{
    uint             crc;
    uint             number;
    SettingsDisplay  display;
    SettingsSystem   system;
    SettingsMeasures measures;

    bool operator!=(const Settings &);
    bool operator==(const Settings &);

    static void Load();

    static void Save();

    static void Reset();

    // ≈сли value больше или меньше уже сохранЄнного измерени€, то перезаписывает старое
    static void SaveMeasure(Measure::E, float value);

    // —брасывает мин и макс значени€ измерени€
    static void ResetMeasure(Measure::E);
};


extern Settings gset;
