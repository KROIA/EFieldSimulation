#pragma once

#include <iostream>
#include <windows.h>
#include "utilities.h"

#define DEBUG_ERROR
#define DEBUG_WARNING
#define DEBUG_MESSAGE



#define STANDARD_CONSOLE_OUTPUT(msg)\
	std::cout<<msg;


enum ConsoleColor
{
    green   = 10,
    hBlue   = 11,
    red     = 12,
    magenta = 13,
    yellow  = 14,
    white   = 15
};

void printColored(const std::string& msg, ConsoleColor color = ConsoleColor::white);
void setConsoleColor(ConsoleColor color);

#if !defined(__PRETTY_FUNCTION__) && !defined(__GNUC__)
#define __PRETTY_FUNCTION__ __FUNCSIG__
#endif





#ifdef DEBUG_ERROR
#define PRINT_ERROR(message)\
    { \
    setConsoleColor(ConsoleColor::red); \
    STANDARD_CONSOLE_OUTPUT("Error in function: ") \
    setConsoleColor(ConsoleColor::magenta); \
    STANDARD_CONSOLE_OUTPUT(__PRETTY_FUNCTION__<<"\n") \
    setConsoleColor(ConsoleColor::red); \
	STANDARD_CONSOLE_OUTPUT("{\n  "<<stringReplaceAll(message,"\n","\n  ")<<"\n}\n") \
    setConsoleColor(ConsoleColor::white); \
    }

#define PTR_CHECK_NULLPTR(ptr,ret) \
    if(ptr == nullptr) \
    {   \
        PRINT_ERROR(std::string(#ptr) + " is a nullptr") \
        ret; \
    }

#else
#define PRINT_ERROR(message)
#endif


#ifdef DEBUG_WARNING
#define PRINT_WARNING(message)\
    { \
    setConsoleColor(ConsoleColor::yellow); \
    STANDARD_CONSOLE_OUTPUT("Warning in function: ") \
    setConsoleColor(ConsoleColor::magenta); \
    STANDARD_CONSOLE_OUTPUT(__PRETTY_FUNCTION__<<"\n") \
    setConsoleColor(ConsoleColor::yellow); \
	STANDARD_CONSOLE_OUTPUT("{\n  "<<stringReplaceAll(message,"\n","\n  ")<<"\n}\n") \
    setConsoleColor(ConsoleColor::white); \
    }
#else
#define PRINT_WARNING(message)
#endif

#ifdef DEBUG_MESSAGE
#define PRINT_MESSAGE(message)\
    STANDARD_CONSOLE_OUTPUT(message)
#define PRINT_FUNCTION \
    STANDARD_CONSOLE_OUTPUT(__PRETTY_FUNCTION__<<"\n")
#define PRINT_FILE \
    STANDARD_CONSOLE_OUTPUT(__FILE__<<"\n")
#else
#define PRINT_MESSAGE(message)
#define PRINT_FUNCTION
#define PRINT_FILE
#endif