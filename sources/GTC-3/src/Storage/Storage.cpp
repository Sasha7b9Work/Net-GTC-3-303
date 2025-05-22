// 2025/05/22 16:18:54 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Storage/Storage.h"
#include "Display/Display.h"
#include "Hardware/InterCom.h"


namespace Storage
{
    // Возвращает указатель на самое "старое" измреение
    static Record *GetOldestRecord();

    static void EraseRecord(Record *);
}


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
    Record *record = GetOldestRecord();

    while (record)
    {
        if (InterCom::Send((TypeMeasure::E)record->type, record->value))
        {
            EraseRecord(record);

            record = GetOldestRecord();
        }
        else
        {
            record = nullptr;
        }
    }
}


Storage::Record *Storage::GetOldestRecord()
{
    return nullptr;
}


void Storage::EraseRecord(Record *)
{

}
