#include <Windows.h>
#include <WindowsX.h>

#include <omni/plot.hpp>

#include <tchar.h>
#include <math.h>

//////////////////////////////////////////////////////////////////////////
/// @brief The main window procedure.
LRESULT CALLBACK mainWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	static omni::plot::XYPlotter *plot_wnd = 0;
	static omni::plot::LineGraph *line = 0;

	if (WM_CREATE == uMsg)
	{
		plot_wnd = new omni::plot::XYPlotter(hWnd, WS_CHILD|WS_VISIBLE, WS_EX_CLIENTEDGE);
		plot_wnd->setScroll(true, true);
		plot_wnd->enableMoving(true);
		plot_wnd->enableZooming(true);

		line = new omni::plot::LineGraph();
		line->getPen()->SetColor(Gdiplus::Color::Red);
		line->setSmoothCurve(true);
		line->setMarker(omni::plot::StarMarker(5));
		line->getMarker()->getPen()->SetColor(Gdiplus::Color::Lime);
		line->getMarker()->setBrush(Gdiplus::SolidBrush(Gdiplus::Color::Blue));
		line->getMarker()->setSize(10.0f);

		for (int i = 0; i < 20; ++i)
			line->push_back(i*0.1f - 1.0f, sinf(i*0.5f));

		plot_wnd->attach(*line);
		plot_wnd->update();
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

	if (WM_MOUSEWHEEL == uMsg)
	{
		return ::SendMessage(plot_wnd->handle(), uMsg, wParam, lParam);
	}

	if (WM_CLOSE == uMsg)
	{
		delete plot_wnd;
		plot_wnd = 0;

		delete line;
		line = 0;

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

	ULONG_PTR gdip_token = 0;
	Gdiplus::GdiplusStartupInput gdip_in;
	Gdiplus::GdiplusStartup(&gdip_token, &gdip_in, 0);

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

	try
	{
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
	}
	catch (const std::exception &e)
	{
		::MessageBoxA(hWnd, e.what(), "Fatal Error",
			MB_OK|MB_ICONERROR);
	}

	Gdiplus::GdiplusShutdown(gdip_token);

	return 0;
}
