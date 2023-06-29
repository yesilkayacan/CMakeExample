#pragma once

#include <string>
#include <iostream>

class Log
{
public:
    class Styles
    {
    public:
        constexpr static std::string_view Default = "\033[0m";
        constexpr static std::string_view Bold = "\033[1m";
        constexpr static std::string_view Dim = "\033[2m";
        constexpr static std::string_view Underlined = "\033[4m";
        constexpr static std::string_view Blink = "\033[5m";
        constexpr static std::string_view Reverse = "\033[7m";
        constexpr static std::string_view Hidden = "\033[8m";
    };

    class Colours
    {
    public:
        constexpr static std::string_view Default = "\033[39m";
        constexpr static std::string_view Black = "\033[30m";
        constexpr static std::string_view Red = "\033[31m";
        constexpr static std::string_view Green = "\033[32m";
        constexpr static std::string_view Yellow = "\033[33m";
        constexpr static std::string_view Blue = "\033[34m";
        constexpr static std::string_view Magenta = "\033[35m";
        constexpr static std::string_view Cyan = "\033[36m";
        constexpr static std::string_view LightGrey = "\033[37m";
        constexpr static std::string_view DarkGrey = "\033[90m";
        constexpr static std::string_view LightRed = "\033[91m";
        constexpr static std::string_view LightGreen = "\033[92m";
        constexpr static std::string_view LightYellow = "\033[93m";
        constexpr static std::string_view LightBlue = "\033[94m";
        constexpr static std::string_view LightMagenta = "\033[95m";
        constexpr static std::string_view LightCyan = "\033[96m";
        constexpr static std::string_view White = "\033[97m";
    };

    template <typename... Args>
    static void Out(Args... args)
    {
        Write(args...);
    }

    template <typename... Args>
    static void Out(const std::string_view &style, const std::string_view &colour, Args... args)
    {
        Write(style, colour, args..., Styles::Default);
    }

    template <typename... Args>
    static void Info(Args... args)
    {
        Out(Styles::Default, Colours::Green, args...);
    }

    template <typename... Args>
    static void Warning(Args... args)
    {
        Out(Styles::Default, Colours::Yellow, args...);
    }

    template <typename... Args>
    static void Error(Args... args)
    {
        Out(Styles::Default, Colours::Red, args...);
    }

private:
    template <typename... Args>
    static void Write(Args... args)
    {
        ((std::cout << std::forward<Args>(args)), ...);
    }
};
