// 2022/04/20 08:53:20 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Display/Display.h"
#include "Hardware/Timer.h"
#include "Display/Colors.h"
#include "Modules/ST7735/ST7735.h"
#include "Modules/W25Q80DV/W25Q80DV.h"
#include "Display/Font/Font.h"
#include "Utils/Text/String.h"
#include "Utils/Text/Text.h"
#include "Display/Zones.h"
#include "Menu/Menu.h"
#include "Settings/Settings.h"
#include "Hardware/HAL/HAL.h"
#include <cstdlib>
#include <cstring>


namespace Display
{
    bool need_redraw = true;

    uint time_prev_redraw = 0;

    static ZoneFPS zoneFPS;

    static Zone *zones[] =
    {
        &zoneFPS,
        nullptr
    };

    void DrawZones();

    struct Measure
    {
        String<> old;
        String<> current;

        ::Measure::E type;
        float value;                // ��������� ������������� ��������
        int position;               // ������� �������������� �������
        uint time;                  // ����� ���������� ��������� ������� �������������� �������

        Measure(::Measure::E t) : type(t), value(0.0f), position(0), time(0) {}

        void Draw(const int x, const int y, int size = 1);

        String<> Name();

        String<> Units();
    };

    static Measure measures[4] =
    {
        Measure(::Measure::Temperature),
        Measure(::Measure::Pressure),
        Measure(::Measure::Humidity),
        Measure(::Measure::DewPoint)
    };

    static void DrawMeasures();

    // ������� ���� ��������� �� ���� �����
    static void DrawBigMeasure();

    static void DrawTime();

    static void DrawStar();

    namespace Buffer
    {
        static uint8 buffer[WIDTH * HEIGHT / 2];       // ������������ ����

        uint8 GetPixels(int x, int y)
        {
            return buffer[(y * WIDTH + x) / 2];
        }

        uint8* GetLine(int x, int y)
        {
            return &buffer[(y * WIDTH + x) / 2];
        }

        static void SetPoint(int x, int y)
        {
            if (x < 0) { return; }
            if (y < 0) { return; }
            if (x >= WIDTH) { return; }
            if (y >= HEIGHT) { return; }

            uint8* pixels = &buffer[(y * WIDTH + x) / 2];

            uint8 value = *pixels;

            if (x & 0x1)
            {
                value &= 0x0F;
                value |= (Color::GetCurrent() << 4);
            }
            else
            {
                value &= 0xF0;
                value |= Color::GetCurrent();
            }

            *pixels = value;
        }

        static void DrawHLine(int y, int x1, int x2)
        {
            if ((x1 % 2) == 0 && (x2 % 2) == 0)
            {
                uint8 *first_byte = &buffer[y * Display::WIDTH / 2];

                uint8 value = (uint8)((uint8)(Color::GetCurrent()) + (Color::GetCurrent() << 4));

                std::memset(first_byte, value, (uint)(x2 - x1) / 2U);
            }
            else
            {
                for (int x = x1; x < x2; x++)
                {
                    Display::Buffer::SetPoint(x, y);
                }
            }
        }

        static void Fill(Color::E color)
        {
            uint8 value = (uint8)((int)(color) | (int)(color << 4));

            std::memset(buffer, value, WIDTH * HEIGHT / 2);
        }
    }
}


void HLine::Draw(int x0, int y, Color::E color)
{
    Color::SetCurrent(color);

    Display::Buffer::DrawHLine(y, x0, x0 + width);
}


void VLine::Draw(int x, int y0, Color::E color)
{
    Color::SetCurrent(color);

    for (int y = y0; y < y0 + height; y++)
    {
        Display::Buffer::SetPoint(x, y);
    }
}


void Point::Set(int x, int y, Color::E color)
{
    Color::SetCurrent(color);

    Display::Buffer::SetPoint(x, y);
}


void Rectangle::Fill(int x0, int y0, Color::E color)
{
    Color::SetCurrent(color);

    for (int x = x0; x < x0 + width; x++)
    {
        VLine(height).Draw(x, y0);
    }
}


void Rectangle::DrawFilled(int x, int y, Color::E fill, Color::E draw)
{
    Rectangle(width, height).Draw(x, y, draw);

    Rectangle(width - 2, height - 2).Fill(x + 1, y + 1, fill);
}


void Rectangle::Draw(int x, int y, Color::E color)
{
    HLine(width).Draw(x, y, color);
    HLine(width).Draw(x, y + height);
    VLine(height).Draw(x, y);
    VLine(height).Draw(x + width, y);
}


void Display::SetMeasure(::Measure::E type, float value)
{
    Settings::SaveMeasure(type, value);

    Measure &measure = measures[type];

    if (value == measure.value) //-V550
    {
        return;
    }

    measure.old.SetFormat(measure.current.c_str());

    measure.position = 0;
    measure.time = TIME_MS;
    measure.value = value;

    measure.current.SetFormat("%.1f", (double)value);
    measure.current[6] = '\0';
}


void Display::Measure::Draw(const int x0, const int y0, int size)
{
    int width_zone = 38;
    int height_zone = 12;
    int y_zone = y0;

    Rectangle(width_zone, height_zone).Fill(x0, y_zone, Color::BLACK);

    Font::Text::DrawBig(x0, y0, size, current.c_str(), Color::WHITE);
}


void Display::DrawZones()
{
    for (int i = 0; true; i++)
    {
        Zone *zone = zones[i];

        if (zone == nullptr)
        {
            break;
        }

        zone->Draw();
    }
}


void Display::BeginScene(Color::E color)
{
    Buffer::Fill(color);
}


void Display::EndScene()
{
    ST7735::WriteBuffer(0, 0, WIDTH, HEIGHT);
}


void Display::Update()
{
    TimeMeterMS meter_fps;

    need_redraw = true;

    if (Menu::Opened())
    {
        Menu::Draw();

        need_redraw = true;
    }
    else
    {
        if (gset.display.typeDisplaydInfo.IsAllMeasures())
        {
            if (need_redraw)
            {
                BeginScene(Color::BLACK);
            }

            DrawMeasures();

            DrawTime();

            DrawStar();

            if (need_redraw)
            {
                EndScene();

                need_redraw = false;
            }

//            DrawZones();
        }
        else
        {
            DrawBigMeasure();
        }

        zoneFPS.string.SetFormat("%02d ms", meter_fps.ElapsedTime());
    }
}


void Display::DrawMeasures()
{
    Font::Set(TypeFont::_12_10);

    const int x0 = 3;

    // ������ ����� ����� ��������
    const int d_lines = (Display::HEIGHT - 5 * Font::Height()) / (5 + 1);

    const int y0 = d_lines;
    const int dY = d_lines + Font::Height();

    static const ::Measure::E types[4] =
    {
        ::Measure::Temperature,
        ::Measure::Pressure,
        ::Measure::Humidity,
        ::Measure::DewPoint
    };

    int y = y0;

    for (int i = 0; i < 4; i++)
    {
        int x = 93;
        int width = 30;
        int height = 15;

        if (need_redraw && measures[types[i]].current.Size())
        {
            String<>("%s", measures[types[i]].Name().c_str()).Draw(x0, y, Color::WHITE);
            measures[types[i]].Units().Draw(x + 41, y);
            measures[types[i]].Draw(x, y);

            ST7735::WriteBuffer(x - 1, y, width, height);
            y += dY;
        }
    }
}


void Display::DrawTime()
{
    int width = 160;
    int height = 16;
    int y = 105;

    Font::Set(TypeFont::_12_10);

    Rectangle(width, height).Fill(4, y - 1, Color::BLACK);

    PackedTime time = HAL_RTC::GetTime();

    String<>("%02d:%02d:%02d", time.hours, time.minutes, time.seconds).Draw(5, 105, Color::WHITE);

    String<>("%02d:%02d:%04d", time.day, time.month, time.year + 2000).Draw(80, 105);

    ST7735::WriteBuffer(0, y, width, height);
}


void Display::DrawStar()
{
    if (((TIME_MS / 500) % 2) == 0)
    {
        String<>("*").Draw(156, 0, Color::WHITE);
    }
}


void Display::DrawBigMeasure()
{
    Font::Set(TypeFont::_8);

    BeginScene(Color::BLACK);

    static const int x[4] =
    {
        15,
        32,
        29,
        20
    };

    Measure &measure = measures[gset.display.typeDisplaydInfo.value];

    Font::Text::DrawBig(x[measure.type], 15, 2, measure.Name().c_str(), Color::_1);

    measures[measure.type].Draw(27, 50, 4);

    Font::Text::DrawBig(68, 95, 2, measure.Units().c_str(), Color::_1);

    EndScene();
}


String<> Display::Measure::Name()
{
    static const pchar names[4] =
    {
        "�����������",
        "��������",
        "���������",
        "����� ����"
    };

    String<> result(names[type]);

    return result;
}

String<> Display::Measure::Units()
{
    static const pchar units[4] =
    {
        "��",
        "���",
        "%%",
        "��"
    };

    return String<>(units[type]);
}
