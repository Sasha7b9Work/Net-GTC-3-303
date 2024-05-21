// 2022/05/06 11:30:11 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Hardware/Keyboard.h"
#include "Hardware/Timer.h"
#include "Menu/Menu.h"
#include <stm32f3xx_hal.h>


Key key1(Key::_1);
Key key2(Key::_2);


namespace Keyboard
{
    static const int TIME_LONG_PRESS = 500;

    static TimeMeterMS meter;

    static bool pressed[Key::Count] = { false, false };        // Если true, клавиша нажата
    static bool taboo_long[Key::Count] = { false, false };     // Если true, запрещено длинное срабатывание

    static bool KeyPressed(Key::E);
    static void UpdateKey(Key::E);

    namespace IT
    {
        static bool pressed[Key::Count] = { false, false };
    }
}


void Keyboard::Init()
{
    /*
    * PB8, PB9
    */

    pinKey1.Init();
    pinKey2.Init();

    HAL_NVIC_SetPriority(EXTI9_5_IRQn, 2, 0);
    HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);
}


void Keyboard::Update()
{
    for (int i = 0; i < Key::Count; i++)
    {
        UpdateKey((Key::E)i);
    }
}


void Keyboard::UpdateKey(Key::E key)
{
    if (meter.ElapsedTime() < 100)
    {
        return;
    }

    if (pressed[key])
    {
        if (meter.ElapsedTime() > TIME_LONG_PRESS && !taboo_long[key])
        {
            Menu::LongPress(key);
            taboo_long[key] = true;
        }
        else
        {
            if (!KeyPressed(key))
            {
                pressed[key] = false;
                meter.Reset();
                if (!taboo_long[key])
                {
                    Menu::ShortPress(key);
                }
                taboo_long[key] = false;
            }
        }
    }
    else
    {
        if (KeyPressed(key))
        {
            pressed[key] = true;
            meter.Reset();
        }
    }
}


bool Keyboard::KeyPressed(Key::E key)
{
    return IT::pressed[key];
}


bool Key::IsPressed() const
{
    return Keyboard::pressed[value];
}


void HAL_GPIO_EXTI_Callback(uint16_t)
{
    Keyboard::IT::pressed[Key::_1] = pinKey1.IsLow();

    Keyboard::IT::pressed[Key::_2] = pinKey2.IsLow();
}
