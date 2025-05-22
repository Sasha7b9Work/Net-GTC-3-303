// 2023/06/12 16:46:45 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


struct TypeMeasure
{
    enum E
    {
        Temperature,        // Температура
        Pressure,           // Давление
        Humidity,           // Влажность
        DewPoint,           // Точка росы
        Count
    };
};


namespace Measures
{
    // Фиксирует последние измерения на экране
    void SetFixed(bool fixed);
    bool IsFixed();
    bool InRange(TypeMeasure::E, float);
}
