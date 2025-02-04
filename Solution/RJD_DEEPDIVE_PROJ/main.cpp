#include <iostream>
#include <Windows.h>

//TO DO
    // - [] Learn about macros
    // - [] Learn about ansi vs unicode
    // - [] 
    // 
    // anytime you use a literal string ("Literal strings give you the ability to use quotations") 
    // in windows it needs to be a WCHAR, you can use L"This is my string" to force UTF-16 encoding


// hInstance - is the handle to an instance or handle to a module.The operating system uses this value to identify the executable or EXE when it's loaded in memory. Certain Windows functions need the instance handle, for example to load icons or bitmaps.
// 
// hPrevInstance - has no meaning.It was used in 16 - bit Windows, but is now always zero.
// 
// pCmdLine - contains the command - line arguments as a Unicode string.
// 
// nCmdShow - is a flag that indicates whether the main application window is minimized, maximized, or shown normally.*/

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam); //forward declaration
bool isSpacePressed = false;

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
    // Register the window class.
    const wchar_t CLASS_NAME[] = L"Sample Window Class"; //L for unicode literal

    WNDCLASS wc = { }; //data structure in windows that describes a window (its not a traditional class)

    wc.lpfnWndProc = WindowProc; //pointer to the Window Proc Function
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;

    RegisterClass(&wc);

    // Create the window.

    HWND hwnd = CreateWindowEx(
        WS_EX_LAYERED | WS_EX_TRANSPARENT,  // Enable transparency, Allow mouse to pass through
        CLASS_NAME,                         // Window class
        L"Learn to Program Windows",        // Controller Heatmap
        WS_POPUP,                           //Borderless style

        // Size and position
        CW_USEDEFAULT, CW_USEDEFAULT, 800, 600,

        NULL,       // Parent window    
        NULL,       // Menu
        hInstance,  // Instance handle
        NULL        // Additional application data
    );

    if (hwnd == NULL)
    {
        return 0;
    }
    SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE); //set the windows as always-on-top
    SetLayeredWindowAttributes(hwnd, RGB(0, 255, 0), 0, LWA_COLORKEY);

    ShowWindow(hwnd, nCmdShow);

    // Run the message loop.

    MSG msg = { };
    while (GetMessage(&msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;

    case WM_PAINT:
    {

        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);
        // All painting occurs here, between BeginPaint and EndPaint.

        
        COLORREF fillColor = isSpacePressed ? RGB(255, 0, 0) : RGB(0, 0, 255); // Red when space is pressed

        HBRUSH transparent = CreateSolidBrush(RGB(0, 255, 0));
        HBRUSH hBrush = CreateSolidBrush(fillColor);
        
        int btnRadius = 30;
        
        HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);
        

        FillRect(hdc, &ps.rcPaint, transparent);
        Ellipse(hdc, 100 - btnRadius, 100 - btnRadius, btnRadius + 100, btnRadius + 100);
        
        
        
        SelectObject(hdc, hOldBrush);
        DeleteObject(hBrush);
        EndPaint(hwnd, &ps);
    }
    case WM_KEYDOWN:
        if (wParam == VK_SPACE && !isSpacePressed)
        {
            isSpacePressed = true;
            InvalidateRect(hwnd, NULL, TRUE); // Force repaint
        }
        break;
    case WM_KEYUP:
        if (wParam == VK_SPACE && isSpacePressed)
        {
            isSpacePressed = false;
            InvalidateRect(hwnd, NULL, TRUE); // Force repaint
        }
        break;
    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    return 0;    
}