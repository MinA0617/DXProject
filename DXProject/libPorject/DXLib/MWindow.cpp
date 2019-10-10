#include "MWindow.h"

MWindow*	g_pWindow = NULL;
HINSTANCE	g_hInstance;
HWND		g_hWnd;
RECT		g_rtWindowClient;

//LRESULT	MWindow::MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
//{
//	return 1;
//}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (g_pWindow != NULL)
	{
		/*g_pWindow->MsgProc(hWnd, msg, wParam, lParam);*/
	}
	switch (msg)
	{
	case WM_SIZE:
	{
		UINT width = LOWORD(lParam);
		UINT height = HIWORD(lParam);
		g_pWindow->ResizeDevice(width, height);
		GetClientRect(g_hWnd, &g_pWindow->m_rtWindowClient);
		g_rtWindowClient = g_pWindow->m_rtWindowClient;
	{
	return true;
}
	}
	case WM_DESTROY:
	{
		PostQuitMessage(0);// WM_QUIT 메세지큐 등록
	}break;
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

MWindow::MWindow()
{
	m_hInstance = NULL;
	m_hWnd = NULL;
	g_pWindow = this;
}


MWindow::~MWindow()
{

}

bool MWindow::InitWindow(HINSTANCE hInstance, LPCWSTR s, int x, int y, int w, int h)
{
	WNDCLASSEX wcex;
	m_hInstance = hInstance;
	g_hInstance = m_hInstance;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.hInstance = hInstance;
	wcex.hbrBackground = (HBRUSH)(DKGRAY_BRUSH);
	wcex.hIcon = NULL;
	wcex.hCursor = NULL;
	wcex.hIconSm = NULL;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.lpfnWndProc = &WndProc;
	wcex.lpszClassName = L"MyClass";
	wcex.lpszMenuName = NULL;

	if (!RegisterClassEx(&wcex))
	{
		return false;
	}

	m_hInstance = hInstance;
	RECT rc;
	rc.left = x;
	rc.right = w;
	rc.top = y;
	rc.bottom = h;
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
	m_hWnd = CreateWindowEx(WS_EX_NOPARENTNOTIFY, L"MyClass", s, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top, NULL, NULL, hInstance, NULL);

	if (!m_hWnd)
	{
		return false;
	}

	m_dwWindowStyle = GetWindowLong(m_hWnd, GWL_STYLE);
	GetWindowRect(m_hWnd, &m_rtWindowBounds);
	GetClientRect(m_hWnd, &m_rtWindowClient);
	CenterWindow(m_hWnd);
	UpdateWindow(m_hWnd);
	m_iWindowWidth = w;
	m_iWindowHeight = h;
	g_hWnd = m_hWnd;
	ShowWindow(m_hWnd, SW_SHOW);

	g_rtWindowClient = m_rtWindowClient;
	return true;
}

bool MWindow::ResizeDevice(UINT width, UINT height)
{
	return true;
}

void MWindow::CenterWindow(HWND hwnd)
{
	// 화면 스크린의 해상도(넓이와 높이)을 얻는다.
	int iScreenWidth = GetSystemMetrics(SM_CXFULLSCREEN);
	int iScreenHeight = GetSystemMetrics(SM_CYFULLSCREEN);

	// 윈도우 클라이언트 중앙과 화면 스크린 중앙을 맞춘다.
	int iDestX = (iScreenWidth - (m_rtWindowBounds.right - m_rtWindowBounds.left)) / 2;
	int iDestY = (iScreenHeight - (m_rtWindowBounds.bottom - m_rtWindowBounds.top)) / 2;

	// 윈도우를 화면중앙으로 이동시킨다.
	MoveWindow(hwnd, iDestX, iDestY,
		m_rtWindowBounds.right - m_rtWindowBounds.left,
		m_rtWindowBounds.bottom - m_rtWindowBounds.top,
		true);
}
