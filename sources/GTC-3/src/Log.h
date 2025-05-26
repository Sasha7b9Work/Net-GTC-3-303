#pragma once


#define LOG_WRITE(...)      Log::Write(Log::Type::Info, __FILE__, __LINE__, __VA_ARGS__)
#define LOG_ERROR(...)      Log::Write(Log::Type::Error, __FILE__, __LINE__, __VA_ARGS__)


namespace Log
{
    struct Type
    {
        enum E
        {
            Info,
            Warning,
            Error
        };
    };

    void Write(Type::E, pchar file, int line, pchar format, ...);
}
