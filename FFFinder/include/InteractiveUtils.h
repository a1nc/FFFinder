#ifndef INTERACTIVE_UTILS_H
#define INTERACTIVE_UTILS_H

#include <Windows.h>
#include <iostream>
#include <string>

const std::string DEFAULT_VOLUME_STR = "\\\\?\\f:";

void InitPrint(int argc, char** argv);

void EndPrint(DWORD nextid, DWORD filecount, DWORD starttick);

WCHAR GetTargetVolumeSymbolFromArgs(int argc, char** argv);

#endif  // INTERACTIVE_UTILS_H
