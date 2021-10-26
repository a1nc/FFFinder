#include "../include/StringUtils.h"

#include <iostream>
#include <stringapiset.h>

using namespace std;

int GetWcharStringSize(const WCHAR* wchars) {
  if (wchars == nullptr) {
    return -1;
  }
  // bytenum contains the '\0'
  return WideCharToMultiByte(CP_OEMCP, NULL, wchars, -1, NULL, 0, NULL, FALSE);
}

int CastWcharToString(const WCHAR* wchars, std::string& result) {
  if (wchars == nullptr) {
    return -1;
  }
  int byteNum = GetWcharStringSize(wchars);
  if (byteNum <= 0) {
    return -1;
  }
  char* buffer = new char[byteNum];
  if (buffer == nullptr) {
    return -1;
  }
  (void)memset(buffer, 0, (byteNum));
  (void)WideCharToMultiByte(CP_OEMCP, NULL, wchars, -1, buffer, byteNum, NULL, FALSE);
  result = string(buffer);
  delete[] buffer;
  return 0;
}

int GetStringWcharSize(const std::string& str) {
  return MultiByteToWideChar(CP_ACP, 0, (LPCSTR)str.c_str(), -1, NULL, 0);
}

int CastStringToWchar(const std::string& str, WCHAR* result, int resultLen) {
  if (result == nullptr || resultLen <= 0) {
    return -1;
  }
  (void)memset(result, 0, resultLen);
  (void)MultiByteToWideChar(CP_ACP, 0, (LPCSTR)str.c_str(), -1, (LPWSTR)result, resultLen);
  return 0;
}

int TestStringUtils(void) {
  string rawMsg("ÄãºÃÊÀ½ç");
  int wcharSize = GetStringWcharSize(rawMsg);
  WCHAR* buffer = new WCHAR[wcharSize];
  (void)CastStringToWchar(rawMsg, buffer, wcharSize);
  int byteSize = GetWcharStringSize(buffer);
  string result = "";
  (void)CastWcharToString(buffer, result);

  cout << "rawMsg : " << rawMsg << endl;
  cout << "wcharSize : " << wcharSize << endl;
  cout << "byteSize : " << byteSize << endl;
  cout << "result : " << result << endl;
  if (result == rawMsg) {
    cout << "Test String Utils Pass ..." << endl;
  } else {
    cout << "Test String Utils Failed ..." << endl;
  }
  return 0;
}
