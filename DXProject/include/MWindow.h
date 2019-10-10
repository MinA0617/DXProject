#pragma once
#include "MStd.h"

class MWindow
{
public:
	MWindow();
	virtual ~MWindow();
public:
	HINSTANCE m_hInstance; // 인스턴스 핸들
	HWND m_hWnd; // 윈도우 핸들

	DWORD m_dwWindowStyle; // 윈도우의 스타일
	RECT m_rtWindowBounds; // 윈도우의 창크기 = 전체크기
	RECT m_rtWindowClient; // 윈도우의 클라이언트 크기 = 타이틀 등등 뺀 크기
	int m_iWindowWidth;
	int m_iWindowHeight;
public:
	bool InitWindow(HINSTANCE hInstance, LPCWSTR s, int x, int y, int w, int h);
	virtual bool ResizeDevice(UINT width, UINT height);
	void CenterWindow(HWND hwnd);
	//virtual LRESULT MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	//void MessageProc(MSG msg);
public:
	//bool Init();
	//bool Frame();
	//bool Render();
	//bool Release();
};