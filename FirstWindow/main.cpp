#include <Windows.h>

LRESULT CALLBACK WindowProc(
	HWND hWnd, // handle to window instance this message is for
	UINT msg, // message id, e.g. WM_CLOSE
	WPARAM wParam, // main message information (single information)
	LPARAM lParam // additional message information (multiple informations)
);

int WINAPI WinMain(
	HINSTANCE hInstance, // handle to our application instance
	HINSTANCE hPrevInstance, // deprecated
	LPSTR szCmdLine, // command line
	int nCmdShow // state how the user want to start the application window (normal, minimised, maximized)
)
{
	// SetProcessDPIAware(); // application is handling dpi for screen resolutions

	UINT width = 768;
	UINT height = 640;

	// 1. create window class
	WNDCLASS wc = {};
	wc.hInstance = hInstance;
	wc.hbrBackground = CreateSolidBrush(RGB(0, 0, 0)); // window background color
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW); // initial window cursor type
	wc.hIcon = LoadIcon(nullptr, IDI_APPLICATION); // initial window icon type
	wc.lpszClassName = TEXT("First Window"); // window class name for later instantiate
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC; // window behaviour (not visual style)
	wc.lpfnWndProc = WindowProc; // communication between windows and our application window

	// 2. register window class
	if (RegisterClass(&wc) == 0) return 10;

	// 3. adjust window rect (optional)
	UINT screenWidth = GetSystemMetrics(SM_CXSCREEN);
	UINT screenHeight = GetSystemMetrics(SM_CYSCREEN);

	// centered window
	RECT windowRect = { 
		(screenWidth - width) / 2, 
		(screenHeight - height) / 2, 
		(screenWidth + width) / 2, 
		(screenHeight + height) / 2
	};
	DWORD style = WS_OVERLAPPEDWINDOW;

	// windowed fullscreen
	//RECT windowRect = { 0, 0, screenWidth, screenHeight	};
	//DWORD style = WS_POPUP;

	AdjustWindowRect(&windowRect, style, false);

	// 4. create window instance
	HWND hWnd = CreateWindow(
		wc.lpszClassName, // window class name
		wc.lpszClassName, // window title
		style, // window visual style
		windowRect.left, windowRect.top, // window position
		windowRect.right - windowRect.left, windowRect.bottom - windowRect.top, // window size
		nullptr, // handle to parent window
		nullptr, // handle to menu instance
		hInstance,
		nullptr // optional parameter
	);
	if (hWnd == nullptr) return 12;

	// 5. show window & set focus
	ShowWindow(hWnd, nCmdShow);
	SetFocus(hWnd);

	// 6. run window
	MSG msg = {};
	while (msg.message != WM_QUIT)
	{
		// GetMessage - return a message, but wait until there is a message available
		// PeekMessage - return a message if there is one availabe or nothing
		// TranslateMessage - translate keyboard message for virtual key table (optional)
		// DispatchMessage - send message to window procedure

		if (PeekMessage(&msg, nullptr, 0, UINT_MAX, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		Sleep(10);
	}

	return msg.wParam;
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
