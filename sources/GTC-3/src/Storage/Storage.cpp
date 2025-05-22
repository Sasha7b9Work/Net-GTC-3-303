// 2025/05/22 16:18:54 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Storage/Storage.h"
#include "Display/Display.h"


void Storage::Init()
{

}


void Storage::Append(TypeMeasure::E type, float value)
{
    if (!Measures::IsFixed())
    {
        Display::SetMeasure(type, value);
    }
}


void Storage::Update()
{

}
