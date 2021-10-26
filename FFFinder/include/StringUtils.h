#ifndef STRING_UTILS_H
#define STRING_UTILS_H

#include <Windows.h>
#include <winnt.h>
#include <string>

int GetWcharStringSize(const WCHAR* wchars);

int CastWcharToString(const WCHAR* wchars, std::string& result);

int GetStringWcharSize(const std::string& str);

int CastStringToWchar(const std::string& str, WCHAR* result, int resultLen);

int TestStringUtils(void);

#endif  // STRING_UTILS_H
