#include "MDxState.h"



MDxState::MDxState()
{
	Init();
}


MDxState::~MDxState()
{
	Release();
}

bool MDxState::Init()
{
	//// √ ±‚»≠ ////
	for (int i = 0; i < MAX_BS; i++)
	{
		m_BlendState[i] = NULL;
	}
	for (int i = 0; i < MAX_RS; i++)
	{
		m_RasterizerState[i] = NULL;
	}
	for (int i = 0; i < MAX_SS; i++)
	{
		m_SamplerState[i] = NULL;
	}
	for (int i = 0; i < MAX_DSS; i++)
	{
		m_DepthStencilState[i] = NULL;
	}
#pragma region BLEND
	HRESULT hr;
	D3D11_BLEND_DESC ds;
	ZeroMemory(&ds, sizeof(D3D11_BLEND_DESC));
	ds.AlphaToCoverageEnable = TRUE;
	ds.IndependentBlendEnable = TRUE;
	ds.RenderTarget[0].BlendEnable = TRUE;						//// ªÁøÎ ø©∫Œ

	ds.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	ds.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;		//// º“Ω∫ «»ºø ¡§∫∏∞™
	ds.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;	//// ø¿«¡Ω∫≈©∏∞¿« «»ºø ¡§∫∏∞™

	ds.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	ds.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	ds.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;

	ds.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	if (FAILED(hr = g_pDevice->CreateBlendState(&ds, &m_BlendState[MAlphaBlend])))
	{
		return false;
	}
#pragma endregion
#pragma region RASTERIZER
	//∑πΩ∫≈Õ∂Û¿Ã¡Ó ªÛ≈¬ ∞¥√º ª˝º∫
	D3D11_RASTERIZER_DESC rsDesc;
	ZeroMemory(&rsDesc, sizeof(rsDesc));
	rsDesc.DepthClipEnable = TRUE;
	rsDesc.FillMode = D3D11_FILL_WIREFRAME;
	rsDesc.CullMode = D3D11_CULL_BACK;
	if (FAILED(hr =	g_pDevice->CreateRasterizerState(&rsDesc, &m_RasterizerState[MWireFrame])))
	{
		return false;
	}
	rsDesc.FillMode = D3D11_FILL_SOLID;
	if (FAILED(hr =	g_pDevice->CreateRasterizerState(&rsDesc, &m_RasterizerState[MSolidFrame])))
	{
		return false;
	}
#pragma endregion
#pragma region	SAMPLER
	D3D11_SAMPLER_DESC ssDesc;
	ZeroMemory(&ssDesc, sizeof(D3D11_SAMPLER_DESC));
	ssDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	ssDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	ssDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	ssDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	ssDesc.MaxLOD = FLT_MAX; // π”∏ ¿« ∏∆Ω∫
	ssDesc.MinLOD = FLT_MIN; // π”∏ ¿« πŒ
	if (FAILED(hr = g_pDevice->CreateSamplerState(&ssDesc, &m_SamplerState[MWrapLinear])))
	{
		return false;
	}
#pragma endregion
#pragma region DEPTHSTENCIL
	D3D11_DEPTH_STENCIL_DESC dssDesc;
	ZeroMemory(&dssDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
	dssDesc.DepthEnable = TRUE;
	dssDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
	dssDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	if (FAILED(hr = g_pDevice->CreateDepthStencilState(&dssDesc, &m_DepthStencilState[MDepthEnable])))
	{
		return false;
	}
	dssDesc.DepthEnable = FALSE;
	if (FAILED(hr = g_pDevice->CreateDepthStencilState(&dssDesc, &m_DepthStencilState[MDepthDisable])))
	{
		return false;
	}
#pragma endregion 
	return true;
}

bool MDxState::Release()
{
	for (int i = 0; i < MAX_BS; i++)
	{
		SAFE_RELEASE(m_BlendState[i]);
	}
	for (int i = 0; i < MAX_RS; i++)
	{
		SAFE_RELEASE(m_RasterizerState[i]);
	}
	for (int i = 0; i < MAX_SS; i++)
	{
		SAFE_RELEASE(m_SamplerState[i]);
	}

	for (int i = 0; i < MAX_DSS; i++)
	{
		SAFE_RELEASE(m_DepthStencilState[i]);
	}
	return true;
}
