#include "MDXWirte.h"
#include "MDevice.h"

void   MDXWirte::Set(HWND hWnd, int iWidth, int iHeight, IDXGISurface1*  pSurface)
{
	CreateDeviceIndependentResources();
	CreateDeviceResources(pSurface);
}
void MDXWirte::DiscardDeviceIndependentResources()
{
	if (m_pTextFormat) m_pTextFormat->Release();
	if (m_pDWriteFactory) m_pDWriteFactory->Release();
	if (m_pd2dFactory) m_pd2dFactory->Release();
}
void MDXWirte::DiscardDeviceResource()
{
	if (m_pRenderTarget) m_pRenderTarget->Release();
	if (m_pBlackBrush) m_pBlackBrush->Release();
}
HRESULT MDXWirte::CreateDeviceResources(IDXGISurface1* pSurface)
{
	//// 3D����Ÿ���� 2D�� ��ȭ�Ͽ� 
	HRESULT hr = S_OK;
	D2D1_RENDER_TARGET_PROPERTIES rp;
	rp.type = D2D1_RENDER_TARGET_TYPE_DEFAULT;
	rp.pixelFormat = D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED);
	rp.dpiX = m_fdpiX;
	rp.dpiY = m_fdpiY;
	rp.usage = D2D1_RENDER_TARGET_USAGE_NONE;
	rp.minLevel = D2D1_FEATURE_LEVEL_DEFAULT;

	if (FAILED(hr = m_pd2dFactory->CreateDxgiSurfaceRenderTarget(pSurface, &rp, &m_pRenderTarget)))
	{
		return hr;
	}
	if (FAILED(hr = m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Black), &m_pBlackBrush)))
	{
		return hr;
	}
	return hr;
}
HRESULT MDXWirte::CreateDeviceIndependentResources()
{
	//// ���丮�� �ؽ�Ʈ ������ �����Ͽ� �����Ѵ� ////
	HRESULT hr = S_OK;
	//// ����|�и�����, �ĺ�ID ////
	hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_pd2dFactory);

	m_pd2dFactory->GetDesktopDpi(&m_fdpiX, &m_fdpiY);
	m_fdpiScaleX = m_fdpiX / 96.0f;
	m_fdpiScaleY = m_fdpiY / 96.0f;

	hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), (IUnknown**)&m_pDWriteFactory);


	////  ��Ʈ��, ��Ʈ���ݷ��Ǹ�, ��Ʈ����, ��Ʈ��Ÿ��, ��Ʈ��Ʈ��ġ(�ø���), ������, ���, ��ȯ������ ////
	//hr = m_pDWriteFactory->CreateTextFormat(L"Gabriola", NULL, DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 20, L"en-us", &m_pTextFormat);
	hr = m_pDWriteFactory->CreateTextFormat(L"�ü�", NULL, DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 20, L"ko-kr", &m_pTextFormat);
	return hr;
}

DWORD MDXWirte::AddData(wstring data, D3DXVECTOR4 color, D3DXVECTOR2 pos)
{
	//// ������ ���� �� �ֵ��� �����ϱ� ////
	TTextArray newtext;
	newtext.data = data;
	newtext.color = color;
	newtext.index = m_dataList.size();
	newtext.rt.top = pos.x;
	newtext.rt.left = pos.y;
	m_dataList.push_back(newtext);
	return newtext.index;
}

void MDXWirte::UpdateData(DWORD index , wstring data, D3DXVECTOR4 color, D3DXVECTOR2 pos)
{
	m_dataList[index].data = data;
	m_dataList[index].color = color;
	m_dataList[index].rt.top = pos.x;
	m_dataList[index].rt.left = pos.y;
}

void MDXWirte::ClearList()
{
	m_dataList.clear();
}

bool MDXWirte::Init()
{
	//// ����ü���� ũ��(����)�� ���ͼ� Dxwirte�� �������ش�. ////
	IDXGISurface1* m_pBuffer = nullptr;
	I_Device.m_pSwapChain->GetBuffer(0, __uuidof(IDXGISurface1), (LPVOID*)&m_pBuffer);
	Set(g_hWnd, g_rtWindowClient.right, g_rtWindowClient.bottom, m_pBuffer);
	SAFE_RELEASE(m_pBuffer);
	return true;
};
bool MDXWirte::Frame()
{

	return true;
};

bool MDXWirte::Render()
{
	if (!m_pRenderTarget) return false;
	m_pRenderTarget->BeginDraw();
	for (int iData = 0; iData < m_dataList.size(); iData++)
	{
		m_pRenderTarget->SetTransform(m_dataList[iData].matWorld);
		D2D1_COLOR_F temp;
		temp.r = m_dataList[iData].color.x;
		temp.g = m_dataList[iData].color.y;
		temp.b = m_dataList[iData].color.z;
		temp.a = m_dataList[iData].color.w;
		m_pBlackBrush->SetColor(temp);
		IDWriteTextFormat*   pTextFormat = m_dataList[iData].pTextFormat;
		if (pTextFormat == nullptr)
		{
			pTextFormat = m_pTextFormat;
		}//fgjfj
#define UNICODE
		//m_pRenderTarget->DrawTextW();
		m_pRenderTarget->DrawText(m_dataList[iData].data.c_str(), m_dataList[iData].data.size(), pTextFormat, m_dataList[iData].rt, m_pBlackBrush);
	}
	m_pRenderTarget->EndDraw();
	return true;
};
bool MDXWirte::Release()
{
	DiscardDeviceResource();
	DiscardDeviceIndependentResources();
	return true;
};

MDXWirte::MDXWirte()
{
}


MDXWirte::~MDXWirte()
{
}
