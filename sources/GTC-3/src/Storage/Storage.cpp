// 2023/09/08 11:47:04 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Storage/Storage.h"


/*
*   В первых двух секторах хранится служебная информация
*/


namespace Storage
{
    struct DataInfo
    {
        uint address_first;     // адрес первой записи
        uint address_last;      // адрес последней записи
    };
}


void Storage::AppendData(const Measurements &)
{

}


bool Storage::IsEmpty()
{
    return false;
}


Measurements *Storage::GetNextData()
{
    return nullptr;
}


void Storage::EraseData(Measurements *)
{

}
