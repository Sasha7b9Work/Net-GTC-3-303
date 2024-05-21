// 2024/05/21 10:54:38 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Utils/Interpolator.h"


float Interpolator::Resolve(float meas)
{
    meas -= begin.measured;

    float k = (end.real - begin.real) / (end.measured - begin.measured);

    return begin.real + meas * k;
}
