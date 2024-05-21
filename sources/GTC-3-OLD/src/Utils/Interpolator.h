// 2024/05/21 10:51:00 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


struct InterPair
{
    InterPair(float _measured, float _real) : measured(_measured), real(_real) { }
    float measured;       // Измеренное значение
    float real;           // Реальное значение
};


class Interpolator
{
public:

    Interpolator(const InterPair &_begin, const InterPair &_end) : begin(_begin), end(_end) { }

    float Resolve(float meas);

private:

    InterPair begin;
    InterPair end;
};
