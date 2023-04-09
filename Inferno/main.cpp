#define WIN32_MEAN_AND_LEAN
#include <Windows.h>

#include "D3d12.h"
#include "d3d12sdklayers.h"
#include "inferno.h"
#include "utils.hpp"

#define                 MAX_LOADSTRING 100 

// GLOBAL VARIABLES 
WCHAR                   szWindowTitle[MAX_LOADSTRING]; 
WCHAR                   szWindowClassName[MAX_LOADSTRING];

BOOL                    bRunning; 
HINSTANCE               hInst;
HWND                    MainWindow;
HACCEL                  AccelTable;

// FORWARD  DECLARATIONS 
LRESULT CALLBACK        WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK        About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

void                    InfernoRegisterClasses(HINSTANCE hInstance);

void                    InfernoInitialize(HINSTANCE hInst);
//void                    Update(); 
//void                    Render(); 
//void                    Destroy();

void                    InfernoGPUInitialize(); 



ComPtr<ID3D12Device>    Device; 




// MAIN FUNCTION

int WINAPI wWinMain(
    _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ PWSTR pCmdLine,
    _In_ int nCmdShow)
{   
 


    try {

        InfernoInitialize(hInstance);
        InfernoGPUInitialize();
    }

    
    catch (const HR_EXCEPTION& ex) {
        MessageBox(NULL, ex.what(), L"FATAL ERROR", MB_OK); 
        return EXIT_FAILURE; 
    }

    //GAME LOOP 
    MSG msg{};
    while (true)
    {
        ShowWindow(MainWindow, SW_SHOW);
        if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) // removes top message from message queue 
        {
            // HANDLE EVENTS OR MESSAGES 
            if (!TranslateAcceleratorW(MainWindow, AccelTable, &msg)) 
            {
                TranslateMessage(&msg);
                DispatchMessageW(&msg);
            }
           
        }
        else
        {
            //PROCESS GAME LOGIC
            //update(); 
            //render(); 
        }
    }



    return 0;
}




// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

//Message Handle funciton 
LRESULT CALLBACK WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_COMMAND : 
        {
            int wmID = LOWORD(wParam); 
            //Parse the menu selections : 
            switch (wmID) {
            case IDM_ABOUT:
                DialogBoxW(hInst, MAKEINTRESOURCE(IDD_DIALOG1), hWnd, About);
                break;

            case ID_FILE_EXIT:
                DestroyWindow(hWnd);
                break;

            default:
                return DefWindowProc(hWnd, msg, wParam, lParam);
            }
        }
        break;
    case WM_DESTROY:
        bRunning = false; 
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, msg, wParam, lParam);
    }
    return 0;
}

void  InfernoRegisterClasses(HINSTANCE hInstance ) 
{
    WNDCLASSEXW wcex;
    wcex.cbSize = sizeof(WNDCLASSEX);  //bytes of window 

    /*
        CS_DBLCLKS - ALLOW DOUBLE CLICKS INSIDE WINDOW
        CS_VREDRAW - DRAW WINDOW AGAIN ON CHANGE OF HEIGHT/WIDTH
    */
    wcex.style = CS_DBLCLKS | CS_VREDRAW | CS_HREDRAW;
    wcex.lpfnWndProc = WindowProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MAINICON));
    wcex.hCursor = LoadCursor(hInstance, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = MAKEINTRESOURCEW(IDR_MENU1);
    wcex.lpszClassName = szWindowClassName;
    wcex.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MAINICON));

    RegisterClassExW(&wcex);

}


void InfernoInitialize(HINSTANCE hInstance) 
{
    AccelTable = LoadAcceleratorsW(hInstance, MAKEINTRESOURCE(IDR_ACCELERATOR1));

    LoadStringW(hInstance, IDS_WINDOWTITLE, szWindowTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_INFERNO, szWindowClassName, MAX_LOADSTRING);

    InfernoRegisterClasses(hInstance);
    hInst = hInstance;
 
    MainWindow  = InfernoCreateWindow(hInstance, 1280, 720, szWindowClassName, szWindowTitle);
}

void InfernoGPUInitialize() 
{

    UINT DxgiFactoryFlags = 0; 

#if !defined(NDEBUG)
    //DEbug controller to check errors in furthter gpu codes
    {
        ComPtr<ID3D12Debug> DebugController;

        if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&DebugController))))
        {
            DebugController->EnableDebugLayer();

            //Enable DXGI Debug Layer 
            DxgiFactoryFlags = DXGI_CREATE_FACTORY_DEBUG;
        }
    }
#endif

    IDXGIFactory4* Factory; 
    ThrowIfFailed(CreateDXGIFactory2(
        DxgiFactoryFlags, 
        IID_PPV_ARGS(&Factory)
    )); 

    IDXGIAdapter1* HardwareAdapter;
 
    HardwareAdapter = GetHardwareAdapter(Factory, DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE);

    ThrowIfFailed(D3D12CreateDevice(
        HardwareAdapter,
        D3D_FEATURE_LEVEL_11_1,
        IID_PPV_ARGS(&Device)
    ));
}



/*
	int main() {
		return 0;
	}
*/
