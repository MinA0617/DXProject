#include "MDevice.h"
#include "MTexture.h"
#include "VertexShaderMgr.h"
#include "PixelShaderMgr.h"
#include "M3DMaterial.h"


ID3D11Device*				g_pDevice;
ID3D11DeviceContext*		g_pImmediateContext;
ID3D11RenderTargetView*		g_pRenderTargetView;
ID3D11DepthStencilView*		g_pDepthStencilView;
D3D11_VIEWPORT				g_ViewPort;

MDevice::MDevice()
{
	m_pSwapChain = NULL;
	m_pDevice = NULL;
	m_pImmediateContext = NULL;
	m_RasterizerStateID = MSolidFrame;
}


MDevice::~MDevice()
{
}


bool MDevice::Init()
{

	if (!CreateDevice()) return false;
	if (!CreateGIFactory()) return false;
	if (!CreateSwapChain()) return false;
	if (!SetRenderTargetView()) return false;
	if (!SetViewPort()) return false;
	if (!I_MaterialMgr.Init()) return false;
	if (!I_RenderTargetMgr.Init()) return false;
	if (!I_VertexShaderMgr.Init()) return false;
	if (!I_PixelShaderMgr.Init()) return false;
	if (!I_DxState.Init()) return false;
	if (!I_3DObjectMgr.Init()) return false;
	m_dxWrite.Init();

	return true;
}

bool MDevice::Release()
{
	I_VertexShaderMgr.Release();
	I_PixelShaderMgr.Release();
	I_DxState.Release();
	I_3DObjectMgr.Release();
	I_RenderTargetMgr.Release();
	I_MaterialMgr.Release();

	SAFE_RELEASE(m_pSwapChain);
	SAFE_RELEASE(m_pDevice);
	SAFE_RELEASE(m_pImmediateContext);
	SAFE_RELEASE(m_pGIFactory);
	m_dxWrite.Release();

	return true;
}

bool MDevice::PreRender()
{
	I_RenderTargetMgr.m_RTList[0]->m_RasterizerStateID = m_RasterizerStateID;
	I_RenderTargetMgr.PreRender();
	return true;
}

bool MDevice::PostRender()
{
	I_RenderTargetMgr.PostRender();
	m_dxWrite.Render();
	m_pSwapChain->Present(0, 0);
	return true;
}

bool MDevice::Render()
{
	I_RenderTargetMgr.Render();
	return true;
}

bool MDevice::Frame()
{
	I_MaterialMgr.Frame();
	I_RenderTargetMgr.Frame();
	m_dxWrite.Frame();
	return true;
}

bool MDevice::CreateDevice()
{
	HRESULT hr;
	UINT  CreateDeviceFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
#ifdef _DEBUG
	CreateDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	D3D_FEATURE_LEVEL pFeatureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_0
	};
	D3D_DRIVER_TYPE dt[] =
	{
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE,
	};
	UINT iNumDriverType = ARRAYSIZE(dt);
	UINT iNumFeatureLevels = ARRAYSIZE(pFeatureLevels);

	for (int iType = 0; iType < iNumDriverType; iType++)
	{
		m_DirverType = dt[iType];
		if (SUCCEEDED(hr = D3D11CreateDevice(
			NULL, m_DirverType, NULL,
			CreateDeviceFlags,
			pFeatureLevels,
			iNumFeatureLevels,
			D3D11_SDK_VERSION,
			&m_pDevice,
			&m_FeatureLevel,//D3D_FEATURE_LEVEL_11_0
			&m_pImmediateContext)))
		{
			if (m_FeatureLevel < D3D_FEATURE_LEVEL_11_0)
			{
				m_pImmediateContext->Release();
				m_pDevice->Release();
				continue;
			}
			break;
		}
	}
	if (FAILED(hr))
	{
		return false;
	}
	g_pDevice = m_pDevice;
	g_pImmediateContext = m_pImmediateContext;
	return true;
}

bool MDevice::CreateGIFactory()
{
	HRESULT hr;
	hr = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&m_pGIFactory);
	if (FAILED(hr))
	{
		return false;
	}
	return true;
}

bool MDevice::CreateSwapChain()
{
	HRESULT hr;
	ZeroMemory(&m_SwapChainDesc, sizeof(m_SwapChainDesc));
	m_SwapChainDesc.BufferCount = 1;
	m_SwapChainDesc.OutputWindow = g_hWnd;
	m_SwapChainDesc.Windowed = true;
	m_SwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	m_SwapChainDesc.SampleDesc.Count = 1;
	m_SwapChainDesc.BufferDesc.Width = g_rtWindowClient.right;
	m_SwapChainDesc.BufferDesc.Height = g_rtWindowClient.bottom;
	m_SwapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	m_SwapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	m_SwapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	hr = m_pGIFactory->CreateSwapChain( m_pDevice, &m_SwapChainDesc, &m_pSwapChain);
	if (FAILED(hr))
	{
		return false;
	}
	g_rtWindowClient.right = m_SwapChainDesc.BufferDesc.Width;
	g_rtWindowClient.bottom = m_SwapChainDesc.BufferDesc.Height;
	return true;
}

bool MDevice::SetRenderTargetView()
{
	HRESULT hr;
	ID3D11Texture2D* pBackBuffer;
	hr = m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
	if (FAILED(hr)) return false;

	hr = m_pDevice->CreateRenderTargetView(pBackBuffer, NULL, &m_pRenderTargetView);
	pBackBuffer->Release();//**********

	if (FAILED(hr)) return false;

	// 깊이스텐실 버퍼 생성
	ID3D11Texture2D* texture;
	D3D11_TEXTURE2D_DESC td;
	ZeroMemory(&td, sizeof(D3D11_TEXTURE2D_DESC));
	td.Width = m_SwapChainDesc.BufferDesc.Width;
	td.Height = m_SwapChainDesc.BufferDesc.Height;
	td.ArraySize = 1;
	td.MipLevels = 1;
	td.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	td.SampleDesc.Count = 1;
	td.SampleDesc.Quality = 0;
	td.Usage = D3D11_USAGE_DEFAULT;
	td.BindFlags = D3D11_BIND_DEPTH_STENCIL;

	hr = m_pDevice->CreateTexture2D(&td, NULL, &texture);

	D3D11_DEPTH_STENCIL_VIEW_DESC dsd;
	ZeroMemory(&dsd, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
	dsd.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	dsd.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;

	hr = m_pDevice->CreateDepthStencilView(texture, &dsd, &m_pDepthStencilView);
	// 랜더타켓 바인딩
	m_pImmediateContext->OMSetRenderTargets(1, &m_pRenderTargetView, m_pDepthStencilView);


	g_pRenderTargetView = m_pRenderTargetView;
	g_pDepthStencilView = m_pDepthStencilView;
	return true;
}

bool MDevice::SetViewPort()
{
	m_ViewPort.Width = m_SwapChainDesc.BufferDesc.Width;
	m_ViewPort.Height = m_SwapChainDesc.BufferDesc.Height;
	m_ViewPort.MinDepth = 0;
	m_ViewPort.MaxDepth = 1;
	m_ViewPort.TopLeftX = 0;
	m_ViewPort.TopLeftY = 0;
	m_pImmediateContext->RSSetViewports(1, &m_ViewPort);

	g_rtWindowClient.right = m_SwapChainDesc.BufferDesc.Width;
	g_rtWindowClient.bottom = m_SwapChainDesc.BufferDesc.Height;

	g_ViewPort = m_ViewPort;
	return true;
}

bool MDevice::ResizeSwapChain(UINT width, UINT height)
{
	if (m_pDevice == nullptr) return true;
	HRESULT hr;
	m_pImmediateContext->OMSetRenderTargets(0, NULL, NULL);

	hr = m_pSwapChain->ResizeBuffers(m_SwapChainDesc.BufferCount, width, height, m_SwapChainDesc.BufferDesc.Format, m_SwapChainDesc.Flags);

	m_pSwapChain->GetDesc(&m_SwapChainDesc);
	//// 수정요망 ////
	return true;
}