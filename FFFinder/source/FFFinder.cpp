// USNOnWin10.cpp : This file contains the 'main' function. Program execution
// begins and ends there.
//
#include "../include/GlobalLimits.h"
#include "../include/InteractiveUtils.h"
#include "../include/StringUtils.h"
#include "../include/UsnUtils.h"

#include <WinIoCtl.h>
#include <Windows.h>
#include <iostream>
#include <set>
#include <string>

using namespace std;

int main(int argc, char **argv) {
  std::ios::sync_with_stdio(false);
  /*
    Support run on windows cmd administartor's mode
    USNOnWin10.exe [volumeSymbol]
    example for scanning the volume C:'s files which run in admin's cmd:
      USNOnWin10.exe c
  */
  TestStringUtils();
  InitPrint(argc, argv);
  string tempVolume("g");
  HANDLE g_Handle = CreateVolumeReadHandle(tempVolume);
  if (g_Handle == INVALID_HANDLE_VALUE) {
    std::cout << "ERROR : Create volume handle error, " << GetLastError();
    return -1;
  }

  DWORD starttick;
  starttick = GetTickCount64();
  void *buffer = VirtualAlloc(NULL, BUFFER_SIZE, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
  if (buffer == NULL) {
    std::cout << "ERROR: Allocate memory error, " << GetLastError();
    return -1;
  }
  DWORD bytecount = 0;
  if (QueryUSNJournal(g_Handle, buffer, BUFFER_SIZE, bytecount) != 0) {
    std::cout << "ERROR: Query USN Journal failed error, " << GetLastError() << std::endl;
    //we create the readonly handle, so can't create USN journl in this volume yet.
    /*std::cout << "INFO: Try to create USN journl in this volume" << std::endl;
    if (CreateUSNJouranl(g_Handle) == -1) {
      return 0;
    }*/
    return -1;
  }

  USN_JOURNAL_DATA *journal = (USN_JOURNAL_DATA *)buffer;
  PrintUSNJournal(journal);
  MFT_ENUM_DATA_V0 mft{0, 0, journal->MaxUsn};
  USN_RECORD *currRecord;
  USN_RECORD *recordend;
  DWORDLONG nextid = 0;
  DWORDLONG filecount = 0;

  for (;;) {
    if (QueryMFTEnumData(g_Handle, mft, buffer, BUFFER_SIZE, bytecount) != 0) {
      EndPrint(nextid, filecount, starttick);
      return 0;
    }
    nextid = *((DWORDLONG *)buffer);
    currRecord = (USN_RECORD *)((USN *)buffer + 1);
    recordend = (USN_RECORD *)(((BYTE *)buffer) + bytecount);
    while (currRecord < recordend) {
      PrintUSNRecord(currRecord, 1);
      filecount++;
      currRecord = (USN_RECORD *)(((BYTE *)currRecord) + currRecord->RecordLength);
    }
    mft.StartFileReferenceNumber = nextid;
  }
  return 0;
}
