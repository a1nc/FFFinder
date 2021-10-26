#include "../include/InteractiveUtils.h"

void InitPrint(int argc, char** argv) {
  std::cout << "========== USN SIMPLE TOOLS ==========" << std::endl;
  std::cout << "Input params : " << std::endl;
  for (int i = 0; i < argc; i++) {
    std::cout << i << " " << argv[i] << std::endl;
  }
  std::cout << "======================================" << std::endl;
}

void EndPrint(DWORD nextid, DWORD filecount, DWORD starttick) {
  std::cout << "======================================" << std::endl;
  std::cout << "LAST ERROR: " << GetLastError() << std::endl;
  std::cout << "Finale ID: " << nextid << std::endl;
  std::cout << "File count: " << filecount << std::endl;
  ULONGLONG endtick = GetTickCount64();
  std::cout << "TICKS: " << endtick - starttick << std::endl;
}

WCHAR GetTargetVolumeSymbolFromArgs(int argc, char** argv) {
  DWORD volumeNameLen = 7;
  DWORD volumeNameKeyPos = volumeNameLen - 3;
  if (argc < 2 || argv == nullptr) {
    return DEFAULT_VOLUME_STR[volumeNameKeyPos];
  }
  std::string volumeSymbol(argv[1]);
  if (volumeSymbol.size() == 1 && isalpha(volumeSymbol[0])) {
    return tolower(volumeSymbol[0]);
  }
  return DEFAULT_VOLUME_STR[volumeNameKeyPos];
}
