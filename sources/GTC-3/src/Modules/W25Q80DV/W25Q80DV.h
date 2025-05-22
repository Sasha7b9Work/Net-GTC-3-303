// 2023/08/07 09:56:54 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


/*

    Память поделена на страницы размером 256 : 4096 страниц по 256 байт.
    Страницы собраны в сектора размером 4 кБ

*/


namespace W25Q80DV
{
    static const uint BEGIN = 0;
    static const uint END = 1024 * 1024;
    static const int SIZE_PAGE = 256;
    static const int SIZE_SECTOR = SIZE_PAGE * 16;

    // Может быть записано не более 256 байт, причём все байты должны быть в пределах одной страницы
    void WriteLess256bytes(uint address, const void *buffer, int size);

    // Размер буфера ограничен 1024 байтами
    void ReadLess1024bytes(uint address, void *buffer, int size);

    void ReadID(uint8 id[2]);

    // Стереть страницу, которой принадлежит address
    void EraseSectorForAddress(uint address);

    void EraseSector(int num_sector);

    void WriteUInt(uint address, uint value);

    void WriteUInt8(uint address, uint8 byte);

    void WriteData(uint, const void *, int);

    uint8 ReadUInt8(uint address);

    namespace Test
    {
        bool Run();

        bool Result();
    }
}
