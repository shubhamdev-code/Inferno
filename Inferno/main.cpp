#define WIN32_MEAN_AND_LEAN
#include <Windows.h>

#include "inferno.h"
#include "utils.hpp"

#define MAX_LOADSTRING 100 

// GLOBAL VARIABLES 
WCHAR szWindowTitle[MAX_LOADSTRING]; 
WCHAR szWindowClassName[MAX_LOADSTRING];
BOOL bRunning; 
HWND hWindow;
HINSTANCE hInst;

// FORWARD  DECLARATIONS 

LRESULT CALLBACK WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam); 
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

// MAIN FUNCTION

int WINAPI wWinMain(
    _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ PWSTR pCmdLine,
    _In_ int nCmdShow)
{   
    hInst = hInstance; 
    LoadStringW(hInstance, IDS_WINDOWTITLE, szWindowTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_INFERNO, szWindowClassName, MAX_LOADSTRING);


    try {
        
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

        //Creates window and a message queue on os and returns window handle 
        hWindow = CreateWindowEx(0,szWindowClassName, szWindowTitle, WS_OVERLAPPEDWINDOW,
            CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);
        
        if (!hWindow) {
            ThrowIfFailed(HRESULT_FROM_WIN32(GetLastError())); 
        }

        bRunning = TRUE;
        ShowWindow(hWindow, SW_SHOW); //displays window

        //Initialize(); 
    }

    
    catch (const HR_EXCEPTION& ex) {
        MessageBox(NULL, ex.what(), L"FATAL ERROR", MB_OK); 
        return EXIT_FAILURE; 
    }
    HACCEL hAccelTale = LoadAcceleratorsW(hInstance, MAKEINTRESOURCE(IDR_ACCELERATOR1));

    //GAME LOOP 
    MSG msg{};
    while (bRunning)
    {
        if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) // removes top message from message queue 
        {
            // HANDLE EVENTS OR MESSAGES 
            if (!TranslateAcceleratorW(hWindow, hAccelTale, &msg)) {
                TranslateMessage(&msg);
                DispatchMessageW(&msg);
            }
           
        }
        else
        {
            //PROCESS GAME LOGIC
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



/*
	int main() {
		return 0;
	}
*/
