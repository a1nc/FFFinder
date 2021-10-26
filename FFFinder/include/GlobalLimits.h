#ifndef GLOBAL_LIMITS_H
#define GLOBAL_LIMITS_H

#include <Windows.h>

constexpr int BUFFER_SIZE = (1024 * 1024);

// "\\?\f:"
constexpr WCHAR VOLUME_ROOT_SYMBOL[] = L"\\\\?\\c:";

constexpr int VOLUME_ROOT_SYMBOL_SIZE = 7;

constexpr int VOLUME_NAME_INDEX = 4;

#endif  // GLOBAL_LIMITS_H
