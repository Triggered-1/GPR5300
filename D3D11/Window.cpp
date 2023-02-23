#include "Window.h"

INT Window::init(HINSTANCE hInstance, UINT width, UINT height)
{
	WNDCLASS wc = {};
	wc.hInstance = hInstance;
	wc.hbrBackground = CreateSolidBrush(RGB(0, 0, 0));
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wc.hIcon = LoadIcon(nullptr, IDI_APPLICATION); 
	wc.lpszClassName = TEXT("Direct3D 11"); 
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC; 
	wc.lpfnWndProc = WindowProc;

	if (RegisterClass(&wc) == 0) return 10;

	UINT screenWidth = GetSystemMetrics(SM_CXSCREEN);
	UINT screenHeight = GetSystemMetrics(SM_CYSCREEN);
	RECT windowRect = {
		(screenWidth - width) / 2,
		(screenHeight - height) / 2,
		(screenWidth + width) / 2,
		(screenHeight + height) / 2
	};
	DWORD style = WS_OVERLAPPEDWINDOW;
	AdjustWindowRect(&windowRect, style, false);

	_hWnd = CreateWindow(wc.lpszClassName, wc.lpszClassName, style, windowRect.left, windowRect.top, 
		windowRect.right - windowRect.left, windowRect.bottom - windowRect.top, nullptr, nullptr, hInstance, nullptr);
	if (_hWnd == nullptr) return 12;

	ShowWindow(_hWnd, SW_SHOW);
	SetFocus(_hWnd);

	return 0;
}

BOOL Window::run()
{
	static MSG msg = {};

	if (PeekMessage(&msg, nullptr, 0, UINT_MAX, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg.message != WM_QUIT;
}

void Window::deInit()
{
}

LRESULT CALLBACK WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_CLOSE:
		DestroyWindow(hWnd);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE) DestroyWindow(hWnd);
		break;

	default:
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
}
