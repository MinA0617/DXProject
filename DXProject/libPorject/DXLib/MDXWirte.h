#pragma once
#include "MStd.h"


struct TTextArray
{
	DWORD index;
	D2D1_MATRIX_3X2_F  matWorld;	// 매트
	D2D1_RECT_F  rt;				// 랙트
	D3DCOLORVALUE color;			// 컬러
	std::wstring data;				// 문자열
	IDWriteTextFormat* pTextFormat;
	TTextArray()
	{
		matWorld = D2D1::IdentityMatrix();
		color = D2D1::ColorF(0, 0, 0, 1);
		rt.top = 0;
		rt.left = 0;
		rt.right = 800;
		rt.bottom = 600;
		pTextFormat = nullptr;
	}
};

class MDXWirte
{
	float m_fdpiX;
	float m_fdpiY;
	float m_fdpiScaleX;
	float m_fdpiScaleY;
private:
	std::vector< TTextArray> m_dataList;
public:
	ID2D1Factory*		 m_pd2dFactory;
	ID2D1RenderTarget*   m_pRenderTarget;
	IDWriteFactory*      m_pDWriteFactory;
	IDWriteTextFormat*   m_pTextFormat;
	IDWriteTextLayout*   m_pTextLayout;
	ID2D1SolidColorBrush* m_pBlackBrush;
public:
	void   Set(HWND hWnd, int iWidth, int iHeight, IDXGISurface1*  pSurface);
private:
	HRESULT CreateDeviceIndependentResources();
	void	DiscardDeviceIndependentResources();

	HRESULT CreateDeviceResources(IDXGISurface1* pSurface);
	void	DiscardDeviceResource();
public:
	DWORD	AddData(wstring data, D3DCOLORVALUE color, D3DXVECTOR2 pos);
	void	UpdateData(DWORD index, wstring data, D3DCOLORVALUE color, D3DXVECTOR2 pos);
	void	ClearList();
public:
	bool	Init();
	bool	Frame();
	bool	Render();
	bool	Release();
public:
	MDXWirte();
	virtual ~MDXWirte();
};

