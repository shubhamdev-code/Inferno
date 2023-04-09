#pragma once


#include "framework.h"

// USING DECLARATIONS
using Microsoft::WRL::ComPtr;

// MACROS
#define MAX_LOADSTRING 100
#define ThrowIfFailed(hr) _ThrowIfFailed(hr, __FILEW__, __LINE__)

// FORWARD DECLARATION
void                        _ThrowIfFailed(HRESULT hr, const wchar_t* const filename, int line);
HWND                        InfernoCreateWindow(HINSTANCE hInst, INT width, INT height, PWSTR className, PWSTR title);


IDXGIAdapter1* GetHardwareAdapter(IDXGIFactory4* Factory, DXGI_GPU_PREFERENCE preference);
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

