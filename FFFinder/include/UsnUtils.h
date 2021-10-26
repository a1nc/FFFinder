#ifndef USN_UTILS_H
#define USN_UTILS_H

#include <Windows.h>
#include <iostream>
#include <set>
#include <string>

HANDLE CreateVolumeReadHandle(const std::string& volume);

int CreateUSNJouranl(const HANDLE handle);

int QueryUSNJournal(const HANDLE handle, void*& buffer, DWORD buffer_size,
  DWORD& bytecount);

int DeleteUSNJouranl(const HANDLE handle);

int QueryMFTEnumData(const HANDLE volume, MFT_ENUM_DATA_V0& mft, void*& buffer,
                      DWORD bufferSize, DWORD& bytecount);

void PrintUSNJournal(USN_JOURNAL_DATA* jounal, DWORD detailLevel = 0);

void PrintUSNJournal(USN_JOURNAL_DATA& jounal, DWORD detailLevel = 0);

void PrintMFT(MFT_ENUM_DATA_V0* mft, DWORD detailLevel = 0);

void PrintMFT(MFT_ENUM_DATA_V0& mft, DWORD detailLevel = 0);

void PrintUSNRecord(USN_RECORD* record, DWORD detailLevel = 0);

void PrintUSNRecord(USN_RECORD& record, DWORD detailLevel = 0);

bool CheckUsnRecord(USN_RECORD* record);

#endif  // USN_UTILS_H
