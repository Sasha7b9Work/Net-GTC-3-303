// 2022/04/27 11:48:13 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Device.h"
#include "Hardware/HAL/HAL.h"
#include "Modules/HC12/HC12.h"
#include "Modules/BME280/BME280.h"
#include "Hardware/CDC/CDC.h"
#include "Modules/ST7735/ST7735.h"
#include "Modules/W25Q80DV/W25Q80DV.h"
#include "Hardware/Timer.h"
#include "Display/Display.h"
#include "Hardware/Keyboard.h"
#include "Hardware/Beeper.h"
#include "Storage/Storage.h"
#include "Hardware/InterCom.h"
#include "Storage/Measures.h"
#include <cmath>


namespace Device
{
    static float CalculateDewPoint(float temperature, float humidity);
    static float CalculateF(float temperature, float humidity);
}


void Device::Init()
{
    HAL::Init();

    gset.Load();

    gset.Reset();

    Storage::Init();

    ST7735::Init();

    BME280::Init();

    HC12::Init();

    Keyboard::Init();

    InterCom::SetDirection((Direction::E)(Direction::CDC | Direction::HC12));

    Beeper::Init();

    while(!W25Q80DV::Test::Run())
    {
    }

    Beeper::Start(2000);
}


void Device::Update()
{
    if (Beeper::Running())
    {
        if (TIME_MS > 5000)
        {
            Beeper::Stop();
        }
    }

    float temp = 0.0f;
    float pressure = 0.0f;
    float humidity = 0.0;

    if (BME280::GetMeasures(&temp, &pressure, &humidity))
    {
        Storage::Append(TypeMeasure::Temperature, temp);
        Storage::Append(TypeMeasure::Pressure, pressure);
        Storage::Append(TypeMeasure::Humidity, humidity);

        float dew_point = CalculateDewPoint(temp, humidity);

        Storage::Append(TypeMeasure::DewPoint, dew_point);

        bool in_range = Measures::InRange(TypeMeasure::Temperature, temp) &&
            Measures::InRange(TypeMeasure::Pressure, pressure) &&
            Measures::InRange(TypeMeasure::Humidity, humidity) &&
            Measures::InRange(TypeMeasure::DewPoint, dew_point);

        if (in_range)
        {
            Beeper::Stop();
        }
        else
        {
            Beeper::Start(100);
        }
    }

    Keyboard::Update();

    Display::Update();

    HAL_ADC::GetVoltage();

    Storage::Update();
}


float Device::CalculateDewPoint(float temperature, float humidity)
{
    float f = CalculateF(temperature, humidity);

    return (237.7f * f) / (17.27f - f);
}


float Device::CalculateF(float temperature, float humidity)
{
    return (17.27f * temperature) / (237.7f + temperature) + std::log(humidity / 100.0f);
}
