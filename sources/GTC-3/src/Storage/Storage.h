// 2023/09/08 11:46:53 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Hardware/HAL/HAL.h"
#include "Modules/W25Q80DV/W25Q80DV.h"


struct Measurements
{
    uint  crc32;
    float temperature;
    float pressure;
    float humidity;
    float dev_point;
    PackedTime time;
};


namespace Storage
{
    static const int SIZE = 1024 * 1024 - 2 * W25Q80DV::SIZE_PAGE;        // 8 MBit = 1 MByte

    void AppendData(const Measurements &);

    bool IsEmpty();

    // Возвращает указатель на следующие данные, если таковые имеются. Не удаляет данные из хранилищца.
    // Чтобы удалить, нужно вызывать EraseData() с указателем на эти данные.
    Measurements *GetNextData();

    void EraseData(Measurements *);
}

