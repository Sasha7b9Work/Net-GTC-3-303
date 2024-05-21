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
#include "Measures.h"

#ifdef GUI
    #include "Hardware/LAN/ClientTCP.h"
#endif


namespace InterCom
{
    /*
    *  Формат сообщения.
    *  0       - 'A'
    *  1       - 'B'
    *  2       - 'C'
    *  3       - type
    *  4 ...11 - ID
    *  12...15 - hash[12...15]
    *  16...19 - value
    */

    Direction::E direction = Direction::_None;

    Buffer<uint8, 20> CreateMessage(TypeMeasure::E type, float value)
    {
        Buffer<uint8, 20> message;

        message[0] = 'A';
        message[1] = 'B';
        message[2] = 'C';
        message[3] = (uint8)type;

        String<> id = HAL::GetUID();

        for (int i = 0; i < 8; i++)
        {
            message[i + 4] = (uint8)id[i];
        }

        std::memcpy(&message[16], &value, 4);

        uint hash = Math::CalculateCRC(&value, 4);

        std::memcpy(&message[12], &hash, 4);

        return message;
    }
}


void InterCom::SetDirection(Direction::E dir)
{
    direction = dir;
}


void InterCom::Send(TypeMeasure::E type, float measure)
{
    static const pchar names[TypeMeasure::Count] =
    {
        "Temperature",
        "Pressure",
        "Humidity",
        "Dew Point"
    };

    static const pchar units[TypeMeasure::Count] =
    {
        "degress Celsius",
        "hPa",
        "%%",
        "degress Celsius"
    };

    if (direction & Direction::Display)
    {
        if (!Measures::IsFixed())
        {
            Display::SetMeasure(type, measure);
        }
    }

    if (direction & Direction::CDC)
    {
        String<> message("%s : %f %s", names[type], measure, units[type]);

        CDC::Transmit(message.c_str(), message.Size() + 1);
    }

    Buffer<uint8, 20> data = CreateMessage(type, measure); //-V821

    if (direction & Direction::HC12)
    {
        HC12::Transmit(data.Data(), data.Size());
    }

#ifdef GUI

    ClientTCP::Transmit(data.Data(), data.Size()); 

#endif
}
