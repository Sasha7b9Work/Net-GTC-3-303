// 2024/05/21 10:51:00 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


struct InterPair
{
    int measured;       // Измеренное значение
    int real;           // Реальное значение
};


class Interpolator
{
public:

    Interpolator(const InterPair &_begin, const InterPair &_end) : begin(_begin), end(_end) { }

private:

    InterPair begin;
    InterPair end;
};
