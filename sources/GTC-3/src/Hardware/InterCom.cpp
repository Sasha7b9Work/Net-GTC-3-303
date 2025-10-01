// Sasha7b9@tut.by (c)
#include "defines.h"
#include "Hardware/InterCom.h"
#include "Hardware/CDC/CDC.h"
#include "Modules/HC12/HC12.h"
#include "Modules/ST7735/ST7735.h"
#include "Display/Display.h"
#include "Utils/Text/String.h"
#include "Utils/Buffer.h"
#include "Utils/Math.h"
#include "Settings/Settings.h"
#include "Hardware/HAL/HAL.h"

#ifdef GUI
    #include "Hardware/LAN/ClientTCP.h"
#endif


namespace InterCom
{
    Direction::E direction = Direction::_None;

    Buffer<16> CreateMessage(Measure::E type, float value)
    {
        Buffer<16> message;

        message[0] = 'A';
        message[1] = 'B';
        message[2] = 'C';
        message[3] = (uint8)type;

        uint id = HAL::GetUID();

        std::memcpy(&message[4], &id, sizeof(id));

        std::memcpy(&message[8], &value, sizeof(value));

        uint hash = Math::CalculateCRC(&message[0], 12);

        std::memcpy(&message[12], &hash, sizeof(hash));

        return message;
    }
}


void InterCom::SetDirection(Direction::E dir)
{
    direction = dir;
}


bool InterCom::Send(Measure::E type, float measure)
{
    static const pchar names[Measure::Count] =
    {
        "Temperature",
        "Pressure",
        "Humidity",
        "Dew Point"
    };

    static const pchar units[Measure::Count] =
    {
        "degress Celsius",
        "hPa",
        "%%",
        "degress Celsius"
    };

    if (direction & Direction::CDC)
    {
        String<> message("%s : %f %s", names[type], (double)measure, units[type]);

        HCDC::Transmit(message.c_str(), message.Size() + 1);
    }

    Buffer<16> data = CreateMessage(type, measure); //-V821

    if (direction & Direction::HC12)
    {
        HC12::Transmit(data.Data(), data.Size());
    }

#ifdef GUI

    ClientTCP::Transmit(data.Data(), data.Size()); 

#endif

    return true;
}
