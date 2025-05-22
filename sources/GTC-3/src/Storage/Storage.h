// 2025/05/22 16:18:32 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Storage/Measures.h"
#include "Hardware/HAL/HAL.h"


namespace Storage
{
    struct Record
    {
        uint            crc32;      // Сюда записывается контрольная сумма для проверки правильности записи. Если 0, то запись стёрта
        PackedTime      time;
        float           value;
        uint            type;       // В этом байте сохранён тип измерения и одновременно остальные нули в старших битах говорят о том, что запись произведена корректно - всё записано
    };

    void Init();

    // Измерение при добавлении в хранилище автоматически отображается на экране
    void Append(TypeMeasure::E, float);

    void Update();
}
