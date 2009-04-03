#include <Windows.h>
#include <WindowsX.h>

#include <omni/plot.hpp>

#include <tchar.h>

//////////////////////////////////////////////////////////////////////////
/// @brief The main window procedure.
LRESULT CALLBACK mainWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	static omni::plot::XYPlotter *plot_wnd = 0;

	if (WM_CREATE == uMsg)
	{
		plot_wnd = new omni::plot::XYPlotter(WS_CHILD|WS_VISIBLE, hWnd);
	}

	if (WM_SIZE == uMsg)
	{
		RECT rc = {0};

		::GetClientRect(hWnd, &rc);
		::InflateRect(&rc, -10, -10);

		::MoveWindow(plot_wnd->handle(),
			rc.left, rc.top, rc.right - rc.left,
			rc.bottom - rc.top, TRUE);
	}

	if (WM_CLOSE == uMsg)
	{
		delete plot_wnd;
		plot_wnd = 0;

		::DestroyWindow(hWnd);
		return 0;
	}

	if (WM_DESTROY == uMsg)
	{
		::PostQuitMessage(0);
		return 0;
	}

	return ::DefWindowProc(hWnd, uMsg, wParam, lParam);
}


//////////////////////////////////////////////////////////////////////////
/// @brief The application entry point.
int WINAPI _tWinMain(HINSTANCE hInst, HINSTANCE, LPTSTR, int showCmd)
{
	LPCTSTR mainWndClass = _T("omni::plot::test::MainWnd");
	HWND hWnd = 0;

	{ // register window class
		WNDCLASSEX wc = {0};
		wc.cbSize = sizeof(wc);
		wc.style = CS_HREDRAW|CS_VREDRAW;
		wc.lpfnWndProc = mainWndProc;
		wc.hInstance = hInst;
		wc.hIcon = ::LoadIcon(NULL, IDI_APPLICATION);
		wc.hCursor = ::LoadCursor(NULL, IDC_ARROW);
		wc.hbrBackground = HBRUSH(COLOR_BTNFACE + 1);
		wc.lpszClassName = mainWndClass;

		if (!::RegisterClassEx(&wc))
			return -1;
	}

	hWnd = ::CreateWindowEx(0, mainWndClass, _T("omni::plot"),
		WS_OVERLAPPEDWINDOW|WS_CLIPCHILDREN, CW_USEDEFAULT, CW_USEDEFAULT,
		CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInst, 0);
	::ShowWindow(hWnd, showCmd);
	::UpdateWindow(hWnd);

	if (hWnd)
	{ // the message loop
		MSG msg = {0};

		while (BOOL ret = ::GetMessage(&msg, NULL, 0, 0))
		{
			//if (ret == -1)
			//	return -1;
			//else
			{
				::TranslateMessage(&msg);
				::DispatchMessage(&msg);
			}
		}
	} // the message loop

	return 0;
}
