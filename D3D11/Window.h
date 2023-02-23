#pragma once
#include <Windows.h>

LRESULT CALLBACK WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam); 

class Window
{
public:
	INT init(HINSTANCE hInstance, UINT width, UINT height);
	BOOL run();
	void deInit();

	HWND getWindowHandle() const { return _hWnd; }

private:
	HWND _hWnd = nullptr; // handle to window instance
};

