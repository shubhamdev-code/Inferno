#define WIN32_MEAN_AND_LEAN
#include <Windows.h>

#include "utils.h"

#define MAX_LOADSTRING 100 

// GLOBAL VARIABLES 
WCHAR szWindowTitle[MAX_LOADSTRING]; 
WCHAR szClassName[MAX_LOADSTRING];
BOOL bRunning; 

// FORWARD  DECLARATIONS 

LRESULT CALLBACK WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam); 

// MAIN FUNCTION

int WINAPI wWinMain(
    _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ PWSTR pCmdLine,
    _In_ int nCmdShow)
{
    try {
        //strcpy for wide string, TEXT() - L type for wide string
        wcscpy_s(szClassName, TEXT("INFERNO_MWC"));
        wcscpy_s(szWindowTitle, TEXT("Main Window"));

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
        wcex.hIcon = LoadIcon(hInstance, IDI_APPLICATION);
        wcex.hCursor = LoadCursor(hInstance, IDC_ARROW);
        wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
        wcex.lpszMenuName = NULL;
        wcex.lpszClassName = szClassName;
        wcex.hIconSm = LoadIcon(hInstance, IDI_APPLICATION);

        RegisterClassExW(&wcex);

        //Creates window and a message queue on os and returns window handle 
        HWND hWindow = CreateWindowEx(0,szClassName, szWindowTitle, WS_OVERLAPPEDWINDOW,
            CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);
        
        if (!hWindow) {
            ThrowIfFailed(HRESULT_FROM_WIN32(GetLastError())); 
        }

        bRunning = TRUE;
        ShowWindow(hWindow, SW_SHOW); //displays window
    }
    
    catch (const HR_EXCEPTION& ex) {
        MessageBox(NULL, ex.what(), L"FATAL ERROR", MB_OK); 
        return EXIT_FAILURE; 
    }

    //GAME LOOP 
    MSG msg{};
    while (bRunning)
    {
        if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) // removes top message from message queue 
        {
            // HANDLE EVENTS OR MESSAGES 
            TranslateMessage(&msg);
            DispatchMessageW(&msg);
        }
        else
        {
            //PROCESS GAME LOGIC
        }
    }

    return 0;
}


LRESULT CALLBACK WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
        case WM_CLOSE:
            DestroyWindow(hWnd);
            bRunning = false;
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
