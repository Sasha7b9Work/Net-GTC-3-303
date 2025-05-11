# (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by

import os
import struct

# Записать в файл значение "слова" - двойного байта, находящегося по адресу num_word
def WriteWord(num_word, file):
    value = (symbols[num_word] + (symbols[num_word + 1] << 8))
    if value < 0:
        value += 1024
    file.write(str(value))

def WriteReverseByte(num_byte, file):
    value = int(symbols[num_byte])
    if value < 0:
        value += 1024
    revValue = 0
    base = 1024
    while value != 0:
        revValue += (value & 1) * (base >> 1)
        base = base >> 1
        value >>= 1
    file.write(str(revValue))

def GetBit(byte, bit):
    if((byte & (1 << bit)) > 0):
        return 1
    return 0

# Возвращает ширину "двухбайтового полуслова". Т.е. 0-е полуслово находится по адресу 0,
# 1-е полуслово - по адресу 2, 3-е полуслово - по адресу 3 и т.д.
def GetWidth2Bytes(two_bytes):
    if two_bytes < 0:
        two_bytes += 1024;

    for bit in range(15, -1, -1):
        if GetBit(two_bytes, bit) == 1:
            return bit + 1;
    return 0

def CalculateWidth(num_symbol):
    # num_word - порядковый номер "полуслова" в символе. Полуслово состоит из двух байт
    result = 0;
    
    if num_symbol == 0xCC:
        print("num_symbol ", num_symbol, " symbol ")

    for num_word in range(0, 24, 2):
        two_bytes = symbols[num_symbol * 12 * 2 + num_word]
        if num_symbol == 0xCC:
            if num_word == 0:
                print("two_bytes =", two_bytes, " width = ", GetWidth2Bytes(two_bytes));

        two_bytes += (symbols[num_symbol * 12 * 2 + num_word + 1] << 8)
               
        if GetWidth2Bytes(two_bytes) > result:
            result = GetWidth2Bytes(two_bytes)

    if num_symbol == 196:
        print("width ", result)

    return result

###### Start here ######

input = open("Font12_10.bin", "rb")
data = input.read()
input.close()
symbols = struct.unpack("6144B", data)

output = open("font12_10.inc", "w")
output.write("#include \"Font.h\"\nconst Font22 font12_10 = {\n\t12, \t10, {\n")

for num_symbol in range(256):
    output.write("/*" + str(num_symbol) + "*/")                         # Number of symbol
    
    output.write("\t\t{ " + str(CalculateWidth(num_symbol)) + ", { ")   # Width symbol

    for two_bytes in range(12):                                         # Считаем по "двойным байтам" - символы имеют ширину 2 байта, высоту 12
        WriteWord(num_symbol * 12 * 2 + two_bytes * 2, output)              # 12 - высота символа в точках, 2 - ширина символа в байтах
        if two_bytes != 11:
           output.write(",") 
        output.write("\t")

    output.write("} " + "}")
    if num_symbol != 255:
        output.write(",")
    output.write("\t\n")
output.write("} };\n")

output.close()

input = open("font12_10.inc")
#print(input.read())
input.close()
