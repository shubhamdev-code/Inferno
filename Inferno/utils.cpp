#include "utils.hpp"
#include <sstream>

// FUNCTION DEFINITIONS
void _ThrowIfFailed(HRESULT hr, const wchar_t* const filename, int line)
{
    static WCHAR errMsg[MAX_LOADSTRING];
    static WCHAR errDesc[MAX_LOADSTRING];
    FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL, GetLastError(), 0,
        (LPWSTR)&errDesc, MAX_LOADSTRING, NULL);

    std::wstringstream wss;
    wss << filename << L":" << line << L"\n\n" << errDesc;
    wcscpy_s(errMsg, wss.str().c_str());
    throw HR_EXCEPTION(hr, errMsg);
}