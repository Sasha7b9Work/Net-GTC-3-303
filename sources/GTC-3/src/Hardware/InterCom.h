// Sasha7b9@tut.by (c)
#pragma once
#include "Display/Display.h"


struct Direction
{
    enum E
    {
        _None   = 0,
        CDC     = (1 << 0),
        HC12    = (1 << 1)
    };
};


namespace InterCom
{
    void SetDirection(Direction::E);

    bool Send(TypeMeasure::E, float measure);
}
