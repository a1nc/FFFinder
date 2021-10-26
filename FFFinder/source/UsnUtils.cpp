#include "../include/UsnUtils.h"

#include <iostream>

#include "../include/GlobalLimits.h"
#include "../include/StringUtils.h"

HANDLE CreateVolumeReadHandle(const std::string& volume) {
  if (volume.empty()) {
    return INVALID_HANDLE_VALUE;
  }
  int bufferSize = GetStringWcharSize(volume);
  WCHAR* wchPtr = new WCHAR[bufferSize];
  if (wchPtr == nullptr) {
    return INVALID_HANDLE_VALUE;
  }
  (void)CastStringToWchar(volume, wchPtr, bufferSize);
  WCHAR volumeWchar = *(wchPtr);
  delete[] wchPtr;

  if (!isalpha(volumeWchar)) {
    std::cout << "ERROR : Symbol error of volume, must A~Z." << std::endl;
    return INVALID_HANDLE_VALUE;
  }

  WCHAR volumeName[VOLUME_ROOT_SYMBOL_SIZE] = {0};
  (void)memcpy(volumeName, VOLUME_ROOT_SYMBOL, VOLUME_ROOT_SYMBOL_SIZE * sizeof(WCHAR));
  volumeName[VOLUME_NAME_INDEX] = volumeWchar;

  HANDLE handle = CreateFile(volumeName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_FLAG_NO_BUFFERING, NULL);
  if (handle == INVALID_HANDLE_VALUE) {
    std::cout << "ERROR : Create volume handle failed." << std::endl;
  }
  return handle;
}

int CreateUSNJouranl(const HANDLE handle) {
  if (handle == INVALID_HANDLE_VALUE) {
    return -1;
  }
  CREATE_USN_JOURNAL_DATA journalData{};
  BOOL ret = DeviceIoControl(handle, FSCTL_CREATE_USN_JOURNAL, &journalData,
                      sizeof(journalData), NULL, 0, 0, NULL);
  if (ret == FALSE) {
    std::cout << "ERROR : Create USN Jouranl failed, error code : " << GetLastError() << std::endl;
    return -1;
  }
  return 0;
}

int QueryUSNJournal(const HANDLE handle, void*& buffer, DWORD bufferSize,
  DWORD& bytecount) {
  BOOL ret = DeviceIoControl(handle, FSCTL_QUERY_USN_JOURNAL, NULL, 0,
    buffer, bufferSize, &bytecount, NULL);
  if (ret == FALSE) {
    return -1;
  }
  return 0;
}

int DeleteUSNJouranl(const HANDLE volume) {
  void* buffer = VirtualAlloc(NULL, BUFFER_SIZE, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
  if (buffer == nullptr) {
    std::cout << "ERROR : Allocate virtual memory failed, error code : " << GetLastError() << std::endl;
    return -1;
  }
  DWORD bytecount = 0;
  if (QueryUSNJournal(volume, buffer, BUFFER_SIZE, bytecount) != 0) {
    std::cout << "ERROR : Query USN Journal failed, error code : " << GetLastError() << std::endl;
    VirtualFree(buffer, 0, MEM_RELEASE);
    return -1;
  }

  USN_JOURNAL_DATA journal = *(USN_JOURNAL_DATA*)buffer;
  DELETE_USN_JOURNAL_DATA deleteJournalData{journal.UsnJournalID, USN_DELETE_FLAG_DELETE};
  // USN_DELETE_FLAG_DELETE or USN_DELETE_FLAG_NOTIFY
  BOOL ret = DeviceIoControl(volume, FSCTL_DELETE_USN_JOURNAL, &deleteJournalData, sizeof(deleteJournalData), NULL, 0, 0, NULL);
  VirtualFree(buffer, 0, MEM_RELEASE);
  if (ret == FALSE) {
    std::cout << "ERROR : Delete USN Jouranl failed, error code : " << GetLastError() << std::endl;
    return -1;
  }
  return 0;
}

int QueryMFTEnumData(const HANDLE volume, MFT_ENUM_DATA_V0& mft, void*& buffer,
                      DWORD bufferSize, DWORD& bytecount) {
  if (volume == INVALID_HANDLE_VALUE) {
    return -1;
  }
  BOOL ret = DeviceIoControl(volume, FSCTL_ENUM_USN_DATA, &mft, sizeof(mft), buffer,
    bufferSize, &bytecount, NULL);
  if (ret == FALSE) {
    return -1;
  }
  return 0;
}

void PrintUSNJournal(USN_JOURNAL_DATA* journal, DWORD detailLevel) {
  if (journal == nullptr) {
    printf("UsnJournalID: null\n");
    return;
  }
  printf("UsnJournalID: %lu\n", journal->UsnJournalID);
  printf("FirstUsn: %lu\n", journal->FirstUsn);
  printf("NextUsn: %lu\n", journal->NextUsn);
  printf("LowestValidUsn: %lu\n", journal->LowestValidUsn);
  printf("MaxUsn: %lu\n", journal->MaxUsn);
  printf("MaximumSize: %lu\n", journal->MaximumSize);
  printf("AllocationDelta: %lu\n", journal->AllocationDelta);
  if (detailLevel > 0) {
    printf("MinMajorVersion: %lu\n", journal->MinSupportedMajorVersion);
    printf("MaxMajorVersion: %lu\n", journal->MaxSupportedMajorVersion);
  }
}

void PrintUSNJournal(USN_JOURNAL_DATA& journal, DWORD detailLevel) {
  PrintUSNJournal(&journal, detailLevel);
}

void PrintMFT(MFT_ENUM_DATA_V0* mft, DWORD detailLevel) {
  if (mft == nullptr) {
    printf("MFT : null\n");
    return;
  }
  printf("StartFileReferenceNumber: %lu\n", mft->StartFileReferenceNumber);
  printf("LowUsn: %lu\n", mft->LowUsn);
  printf("HighUsn: %lu\n", mft->HighUsn);
}

void PrintUSNRecord(USN_RECORD* record, DWORD detailLevel) {
  if (record == nullptr) {
    printf("RecordLength: null\n");
    return;
  }
  printf("=================================================================\n");
  printf("RecordLength: %u\n", record->RecordLength);

  if (detailLevel > 0) {
    printf("MajorVersion: %u\n", (DWORD)record->MajorVersion);
    printf("MinorVersion: %u\n", (DWORD)record->MinorVersion);
  }
  printf("FileReferenceNumber: %lu\n", record->FileReferenceNumber);
  printf("ParentFRN: %lu\n", record->ParentFileReferenceNumber);
  printf("USN: %lu\n", record->Usn);

  if (detailLevel > 0) {
    printf("Timestamp: %lu\n", record->TimeStamp);
    printf("Reason: %u\n", record->Reason);
    printf("SourceInfo: %u\n", record->SourceInfo);
    printf("SecurityId: %u\n", record->SecurityId);
    printf("FileAttributes: %x\n", record->FileAttributes);
  }
  printf("FileNameLength: %u\n", (DWORD)record->FileNameLength);
  WCHAR* filename;
  WCHAR* filenameend;
  filename = (WCHAR*)(((BYTE*)record) + record->FileNameOffset);
  filenameend = (WCHAR*)(((BYTE*)record) + record->FileNameOffset +
                         record->FileNameLength);
  printf("FileName: %.*ls\n", filenameend - filename, filename);
}

void PrintUSNRecord(USN_RECORD& record, DWORD detailLevel) {
  PrintUSNRecord(&record, detailLevel);
}

void PrintMFT(MFT_ENUM_DATA_V0& mft, DWORD detailLevel) {
  PrintMFT(&mft, detailLevel);
}

bool CheckUsnRecord(USN_RECORD* record) {
  WCHAR* filename;
  WCHAR* filenameend;

  filename = (WCHAR*)(((BYTE*)record) + record->FileNameOffset);
  filenameend = (WCHAR*)(((BYTE*)record) + record->FileNameOffset +
                         record->FileNameLength);

  if (filenameend - filename != 8) {
    return false;
  }

  if (wcsncmp(filename, L"test.txt", 8) != 0) {
    return false;
  }
  return true;
}
