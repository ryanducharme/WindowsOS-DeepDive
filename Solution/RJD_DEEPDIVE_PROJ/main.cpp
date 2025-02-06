#include <iostream>
#include <Windows.h>
#include <Xinput.h>
#include <string>
#include <synchapi.h>
#pragma comment(lib,"XInput.lib")


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

struct ButtonData {
	int A = 0;
	int B = 0;
	int X = 0;
	int Y = 0;

	int UP = 0;
	int DOWN = 0;
	int LEFT = 0;
	int RIGHT = 0;
};


void CheckButtons(WORD buttons, ButtonData& data);
void HandleKeystroke(const XINPUT_KEYSTROKE& keystroke, ButtonData& data);

//Keystroke info
XINPUT_STATE state = {};
DWORD controllerId = 0; // Usually, 0 is for the first controller
DWORD playerIndex{};
DWORD reserve{};
RECT rect = { 50, 50, 300, 200 };
XINPUT_KEYSTROKE keystroke;
ButtonData buttonStats{};

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam); //forward declaration
bool isSpacePressed = false;

int WINAPI wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR lpCmdLine,
	_In_ int nShowCmd)
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

	ShowWindow(hwnd, nShowCmd);

	// Run the message loop.
	MSG msg = { };
	while (true)
	{
		while (GetMessage(&msg, NULL, 0, 0) > 0)
		{
			DWORD result = XInputGetState(controllerId, &state);

			if (result == ERROR_SUCCESS) {
				// Controller is connected
				//std::cout << "Controller " << controllerId << " is connected." << std::endl;
				CheckButtons(state.Gamepad.wButtons, buttonStats);

			}
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		InvalidateRect(hwnd, NULL, true);
		Sleep(1);
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

		COLORREF fillColor = isSpacePressed ? RGB(255, 0, 0) : RGB(0, 0, 255); // Red when space is pressed

		HBRUSH transparent = CreateSolidBrush(RGB(0, 255, 0));
		HBRUSH hBrush = CreateSolidBrush(fillColor);

		int btnRadius = 30;

		HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);


		FillRect(hdc, &ps.rcPaint, transparent);
		Ellipse(hdc, 100 - btnRadius, 100 - btnRadius, btnRadius + 100, btnRadius + 100);

		
		std::wstring text = L"A: " + std::to_wstring(buttonStats.A);

   		DrawText(hdc, text.c_str(), -1, &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);


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
	// Get the state of the controller

	return 0;
}

void HandleKeystroke(const XINPUT_KEYSTROKE& keystroke, ButtonData& data)
{


	if (keystroke.Flags & XINPUT_KEYSTROKE_KEYUP) {

		//FACE BUTTONS
		if (keystroke.VirtualKey == VK_PAD_A)
		{
			data.A += 1;
		}
		if (keystroke.VirtualKey == VK_PAD_B)
		{
			data.B += 1;
		}
		if (keystroke.VirtualKey == VK_PAD_X)
		{
			data.X += 1;
		}
		if (keystroke.VirtualKey == VK_PAD_Y)
		{
			data.Y += 1;
		}

		//DPAD
		if (keystroke.VirtualKey == VK_PAD_DPAD_UP)
		{
			data.UP += 1;
		}
		if (keystroke.VirtualKey == VK_PAD_DPAD_DOWN)
		{
			data.DOWN += 1;
		}
		if (keystroke.VirtualKey == VK_PAD_DPAD_LEFT)
		{
			data.LEFT += 1;
		}
		if (keystroke.VirtualKey == VK_PAD_DPAD_RIGHT)
		{
			data.RIGHT += 1;
		}

		if (keystroke.VirtualKey == VK_PAD_START)
		{
			//PrintData(data);
		}
	}
	/*else if (keystroke.Flags & XINPUT_KEYSTROKE_KEYUP) {
		std::cout << "released." << std::endl;
	}*//*
	else if (keystroke.Flags & XINPUT_KEYSTROKE_REPEAT) {
		std::cout << "held down." << std::endl;
	}*/
}

void CheckButtons(WORD buttons, ButtonData& data)
{

	//FACE BUTTONS
	if (buttons & XINPUT_GAMEPAD_A) {
		//std::cout << "A button is pressed!" << std::endl;
 		data.A += 1;
	}
	if (buttons & XINPUT_GAMEPAD_B) {
		std::cout << "B button is pressed!" << std::endl;
		data.B += 1;
	}
	if (buttons & XINPUT_GAMEPAD_X) {
		std::cout << "X button is pressed!" << std::endl;
		data.X += 1;
	}
	if (buttons & XINPUT_GAMEPAD_Y) {
		std::cout << "Y button is pressed!" << std::endl;
		data.Y += 1;
	}

	//DPAD
	if (buttons & XINPUT_GAMEPAD_DPAD_UP) {
		std::cout << "UP button is pressed!" << std::endl;
	}
	if (buttons & XINPUT_GAMEPAD_DPAD_DOWN) {
		std::cout << "DOWN button is pressed!" << std::endl;
	}
	if (buttons & XINPUT_GAMEPAD_DPAD_LEFT) {
		std::cout << "Left button is pressed!" << std::endl;
	}
	if (buttons & XINPUT_GAMEPAD_DPAD_RIGHT) {
		std::cout << "Right button is pressed!" << std::endl;
	}
	if (buttons & XINPUT_GAMEPAD_START) {
		std::cout << data.A << " " << data.B << " " << data.X << " " << data.Y << std::endl;
	}

}