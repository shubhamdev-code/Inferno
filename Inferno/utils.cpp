#include "utils.hpp"
#include <sstream>

// FUNCTION DEFINITIONS
void _ThrowIfFailed(HRESULT hr, const wchar_t* const filename, int line)
{
    if (!hr) return; 
    static WCHAR errMsg[MAX_LOADSTRING];
    static WCHAR errDesc[MAX_LOADSTRING];
    FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL, GetLastError(), 0,
        (LPWSTR)&errDesc, MAX_LOADSTRING, NULL);

    std::wstringstream wss;
    wss << filename << L":" << line << L"\n\n" << errDesc;
    wcscpy_s(errMsg, wss.str().c_str());
    throw HR_EXCEPTION(hr, errMsg);
}

//create window function
HWND InfernoCreateWindow(HINSTANCE hInstance, INT width, INT height, PWSTR className, PWSTR title)
{
    RECT clientRect{ 0, 0, width, height };
    AdjustWindowRect(&clientRect, WS_OVERLAPPEDWINDOW, FALSE);
    width = clientRect.right - clientRect.left;
    height = clientRect.bottom - clientRect.top;

    //Creates window and a message queue on os and returns window handle 
    HWND hWindow = CreateWindowEx(0, className, title, WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0, width, height, NULL, NULL, hInstance, NULL);

    if (!hWindow) {
        ThrowIfFailed(HRESULT_FROM_WIN32(GetLastError()));
    }
    return hWindow;
}


IDXGIAdapter1* GetHardwareAdapter(IDXGIFactory4* Factory, DXGI_GPU_PREFERENCE preference) {
    ComPtr<IDXGIFactory6> TFactory = nullptr; 
    IDXGIAdapter1* Adapter = nullptr; 

    if (SUCCEEDED(Factory->QueryInterface(TFactory.GetAddressOf()))){
        UINT adapterIndex = 0; 
        while (
            SUCCEEDED(TFactory->EnumAdapterByGpuPreference(adapterIndex,preference,IID_PPV_ARGS(&Adapter))))
        {

            DXGI_ADAPTER_DESC1 Desc; 
            Adapter->GetDesc1(&Desc); 

            if (Desc.Flags & DXGI_ADAPTER_FLAG3_SOFTWARE)
                continue;


            if (SUCCEEDED(D3D12CreateDevice(
                Adapter,
                D3D_FEATURE_LEVEL_11_1,
                _uuidof(ID3D12Device),
                nullptr
            ))) {
                break;
            }
            adapterIndex++; 
        }
    }
    
    if (!Adapter) {
        UINT adapterIndex = 0; 
        while (
            SUCCEEDED(Factory->EnumAdapters1(adapterIndex, &Adapter))
            ) {
            DXGI_ADAPTER_DESC1 Desc;
            Adapter->GetDesc1(&Desc);

            if (Desc.Flags & DXGI_ADAPTER_FLAG3_SOFTWARE)
                continue;


            if (SUCCEEDED(D3D12CreateDevice(
                Adapter,
                D3D_FEATURE_LEVEL_11_1,
                _uuidof(ID3D12Device),
                nullptr
            ))) {
                break;
            }
            adapterIndex++;
        }
    }
    return Adapter;
}
