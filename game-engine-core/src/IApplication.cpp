#include "../include/IApplication.h"

IApplication::IApplication() :
	m_iWidth(0),
	m_iHeight(0),
	m_Window(nullptr)
{
}

IApplication::~IApplication()
{
}

bool IApplication::Create(int32_t resX, int32_t resY, const std::string& title)
{
	m_Window = MakeWindow(resX, resY, title);
	if (m_Window)
	{
		m_iWidth = resX;
		m_iHeight = resY;
		return true;
	}

	return false;
}

void IApplication::Run()
{
	MSG msg;
	BOOL gotMsg = false;

	::PeekMessage(&msg, nullptr, 0, 0, PM_NOREMOVE);

	while (msg.message != WM_QUIT)
	{
		gotMsg = ::GetMessage(&msg, nullptr, 0, 0);
		if (gotMsg)
		{
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
		}
	}
}

void IApplication::Debug(const wchar_t* msg)
{
	::OutputDebugStringW(msg);
}

void IApplication::Debug(const char* msg)
{
	::OutputDebugStringA(msg);
}

void IApplication::Debug(const std::string& msg)
{
	OutputDebugStringA(msg.c_str());
}

HWND IApplication::MakeWindow(int32_t resX, int32_t resY, const std::string& title)
{
	HINSTANCE inst = GetModuleHandle(nullptr);
	DWORD windowStyle = WS_OVERLAPPED
			| WS_CAPTION
			| WS_SYSMENU
			| WS_THICKFRAME
			| WS_MINIMIZEBOX
			| WS_MAXIMIZEBOX;

	WNDCLASS wc;
	memset(&wc, 0, sizeof(WNDCLASS)); // Allocate zeroes for size of WNDCLASS
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = (WNDPROC)WndProc;
	wc.hInstance = inst;
	wc.hIcon = ::LoadIcon(nullptr, IDI_APPLICATION);
	wc.hCursor = ::LoadCursor(nullptr, IDC_ARROW);
	wc.hbrBackground = (HBRUSH) ::GetStockObject(BLACK_BRUSH);
	wc.lpszClassName = L"GRAPHICSENGINE_WNDCLASS";

	if (!::RegisterClass(&wc))
	{
		Debug(L"Failed to register window class, exiting...\n");
		return nullptr;
	}

	// Get window title as wide string
	auto widestr = std::wstring(title.begin(), title.end());

	// Create window
	HWND window = ::CreateWindow(
		wc.lpszClassName, // window classname
		widestr.c_str(), // window title
		windowStyle,	// window style
		CW_USEDEFAULT, // X position
		CW_USEDEFAULT, // Y position
		resX, // width
		resY, // height
		nullptr, // no parent window
		nullptr, // no menus
		inst, // instance
		nullptr); // no extra parameters

	if (!window)
	{
		Debug(L"Failed to create window, exiting...\n");
		return nullptr;
	}

	// Refresh window style
	::SetWindowLong(window, GWL_STYLE, windowStyle);

	// Adjust window size to fit the requested resolution
	RECT clientArea = { 0, 0, resX, resY };
	::AdjustWindowRectEx(&clientArea, windowStyle, FALSE, 0);
	::SetWindowPos(
		window,
		nullptr,
		0, 0,
		clientArea.right - clientArea.left,
		clientArea.bottom - clientArea.top,
		SWP_NOZORDER | SWP_NOMOVE | SWP_SHOWWINDOW);

	// Force update and show window
	::UpdateWindow(window);
	::ShowWindow(window, SW_SHOWNORMAL);

	return window;
}

long __stdcall IApplication::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
		case WM_DESTROY: // Window is being destroyed
			::PostQuitMessage(0);
			return 0;
			break;

		case WM_CREATE: // Window has been created
			::SetForegroundWindow(hWnd);
			break;

		case WM_CLOSE: // Window is being closed
			::DestroyWindow(hWnd);
			return 0;
			break;

		default:
			break;
	}

	return (long)::DefWindowProc(hWnd, msg, wParam, lParam);
}
