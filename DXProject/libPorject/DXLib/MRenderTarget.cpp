#include "MRenderTarget.h"

MRenderTarget::MRenderTarget()
{
	m_RasterizerStateID = MSolidFrame;
}


MRenderTarget::~MRenderTarget()
{
}


bool MRenderTarget::Create(UINT width, UINT height)
{
	HRESULT hr;
	D3D11_TEXTURE2D_DESC td;
	ZeroMemory(&td, sizeof(D3D11_TEXTURE2D_DESC));
	td.Width = width;
	td.Height = height;
	td.MipLevels = 1;
	td.SampleDesc.Count = 1;
	td.SampleDesc.Quality = 0;
	td.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	td.Usage = D3D11_USAGE_DEFAULT;
	td.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	td.ArraySize = 1;
	hr = g_pDevice->CreateTexture2D(&td, NULL, &m_pTexture);
	if (FAILED(hr)) return false;
	hr = g_pDevice->CreateShaderResourceView(m_pTexture, NULL, &m_pShaderResourceView);
	if (FAILED(hr)) return false;
	hr = g_pDevice->CreateRenderTargetView(m_pTexture, NULL, &m_pRenderTargetView);
	if (FAILED(hr)) return false;

	td.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	td.BindFlags = D3D11_BIND_DEPTH_STENCIL;

	hr = g_pDevice->CreateTexture2D(&td, NULL, &m_pDSTexture);
	// ½ºÅÙ½Çºä ////
	D3D11_DEPTH_STENCIL_VIEW_DESC dsd;
	ZeroMemory(&dsd, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
	dsd.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	hr = g_pDevice->CreateDepthStencilView(m_pDSTexture, &dsd, &m_pDepthStencilView);
	if (FAILED(hr)) return false;

	m_ViewPort.Width = width;
	m_ViewPort.Height = height;
	m_ViewPort.MinDepth = 0;
	m_ViewPort.MaxDepth = 1;
	m_ViewPort.TopLeftX = 0;
	m_ViewPort.TopLeftY = 0;

	return true;
}

bool MRenderTarget::Begin()
{
	I_DxState.RS_Set(m_RasterizerStateID);

	m_iViewports = 1;
	g_pImmediateContext->RSGetViewports(&m_iViewports, &m_OldViewPort);
	g_pImmediateContext->OMGetRenderTargets(1, &m_pOldRenderTargetView, &m_pOldDepthStencilView);

	g_pImmediateContext->ClearRenderTargetView(m_pRenderTargetView, D3DXVECTOR4(0.5, 0.5, 0.5, 1));
	g_pImmediateContext->ClearDepthStencilView(m_pDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	ID3D11RenderTargetView* temp = NULL;
	ID3D11DepthStencilView* temp2 = NULL;
	g_pImmediateContext->OMSetRenderTargets(1, &temp, temp2);
	g_pImmediateContext->OMSetRenderTargets(1, &m_pRenderTargetView, m_pDepthStencilView);
	g_pImmediateContext->RSSetViewports(1, &m_ViewPort);

	return true;
}

bool MRenderTarget::End()
{
	ID3D11RenderTargetView* temp = NULL;
	ID3D11DepthStencilView* temp2 = NULL;
	g_pImmediateContext->OMSetRenderTargets(1, &temp, temp2);

	g_pImmediateContext->RSSetViewports(m_iViewports, &m_OldViewPort);
	g_pImmediateContext->OMSetRenderTargets(1, &m_pOldRenderTargetView, m_pOldDepthStencilView);


	m_pOldRenderTargetView = NULL;
	m_pOldDepthStencilView = NULL;


	I_DxState.RS_Set(MSolidFrame);
	return false;
}

bool    MRenderTarget::Release()
{
	SAFE_RELEASE(m_pTexture);
	SAFE_RELEASE(m_pShaderResourceView);
	SAFE_RELEASE(m_pRenderTargetView);
	SAFE_RELEASE(m_pDepthStencilView);
	SAFE_RELEASE(m_pDSTexture);
	return true;
}