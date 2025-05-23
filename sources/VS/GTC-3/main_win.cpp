// 2022/04/29 10:12:30 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#define WIN32_LEAN_AND_MEAN
#include "defines.h"
#include "GUI/Application.h"
#include "GUI/Frame.h"
#include "Device.h"
#include "Hardware/LAN/ClientTCP.h"
#include "Hardware/Timer.h"
#include "GUI/ConsoleGUI.h"


void Application::Init()
{
    Device::Init();

    ConsoleGUI::Create();

    ConsoleGUI::self->Show(true);
}


void Application::Update()
{
    static TimeMeterMS meter;

    if (!ClientTCP::Connected() && meter.ElapsedTime() > 1000)
    {
        meter.Reset();

        ClientTCP::Connect(777);
    }

    Device::Update();
}
