#pragma once
#include "MStd.h"

class MWindow
{
public:
	MWindow();
	virtual ~MWindow();
public:
	HINSTANCE m_hInstance; // �ν��Ͻ� �ڵ�
	HWND m_hWnd; // ������ �ڵ�

	DWORD m_dwWindowStyle; // �������� ��Ÿ��
	RECT m_rtWindowBounds; // �������� âũ�� = ��üũ��
	RECT m_rtWindowClient; // �������� Ŭ���̾�Ʈ ũ�� = Ÿ��Ʋ ��� �� ũ��
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