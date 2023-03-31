#pragma once

#include <dxgi1_6.h>
#include <d3d12.h>

#include <string>
#include <sstream>

// MACROS
#define MAX_LOADSTRING 100
#define ThrowIfFailed(hr) _ThrowIfFailed(hr, __FILEW__, __LINE__)

// FORWARD DECLARATION
void _ThrowIfFailed(HRESULT hr, const wchar_t* const filename, int line);

// CLASS DEFINITIONS
class HR_EXCEPTION
{
public:
    HR_EXCEPTION(const wchar_t* const what)
        : Hr{}, What{ what } {}
    HR_EXCEPTION(HRESULT hr, const wchar_t* const what)
        :Hr{ hr }, What{ what } {}

    const wchar_t* const what() noexcept { return What; }

    const wchar_t* const what() const noexcept { return What; }

private:
    HRESULT Hr;
    const wchar_t* const What;
};

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
