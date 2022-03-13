#include "debugHelper.h"



void printColored(const std::string& msg, ConsoleColor color)
{
    setConsoleColor(color);
    STANDARD_CONSOLE_OUTPUT(msg)
    setConsoleColor(ConsoleColor::white);
}
void setConsoleColor(ConsoleColor color)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
}