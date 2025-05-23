// 2025/05/22 16:18:54 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Storage/Storage.h"
#include "Display/Display.h"
#include "Hardware/InterCom.h"


namespace Storage
{
    namespace Memory
    {
        static bool IsFull();

        // —тереть самую старую запись
        static void EraseOldestRecord();

        // ¬озвращает указатель на самое "старое" измерение
        static Record *GetOldestRecord();

        static void EraseRecord(Record *);

        static void Append(TypeMeasure::E, float);
    }
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

    if (Memory::IsFull())
    {
        Memory::EraseOldestRecord();
    }

    Memory::Append(type, value);
}


void Storage::Update()
{
    Record *record = Memory::GetOldestRecord();

    while (record)
    {
        if (InterCom::Send((TypeMeasure::E)record->type, record->value))
        {
            Memory::EraseRecord(record);

            record = Memory::GetOldestRecord();
        }
        else
        {
            record = nullptr;
        }
    }
}


Storage::Record *Storage::Memory::GetOldestRecord()
{
    LOG_ERROR("Not implemented");

    return nullptr;
}


void Storage::Memory::EraseRecord(Record *)
{
    LOG_ERROR("Not implemented");
}


void Storage::Memory::Append(TypeMeasure::E, float)
{
    LOG_ERROR("Not implemented");
}


void Storage::Memory::EraseOldestRecord()
{
    LOG_ERROR("Not implemented");
}


bool Storage::Memory::IsFull()
{
    LOG_ERROR("Not implemented");

    return true;
}
