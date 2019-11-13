#pragma once
#include "MStd.h"
#include "MDxState.h"
#include "M2DScreen.h"

class MRenderTarget
{
public:
	ID3D11Texture2D*			m_pTexture;				//// 랜더타켓 텍스쳐
	ID3D11ShaderResourceView*	m_pShaderResourceView;	////
	ID3D11RenderTargetView*		m_pRenderTargetView;	//// 랜더타켓 뷰 = 오프스크린
	ID3D11DepthStencilView*		m_pDepthStencilView;	//// 스탠실뷰
	ID3D11Texture2D*			m_pDSTexture;			////
	D3D11_VIEWPORT				m_ViewPort;				//// 메인스크린의 속성값
	// --------------------------------------------------------------------------------------------
	// old data 
	// --------------------------------------------------------------------------------------------
	UINT						m_iViewports;
	ID3D11RenderTargetView*		m_pOldRenderTargetView;
	ID3D11DepthStencilView*		m_pOldDepthStencilView;
	D3D11_VIEWPORT				m_OldViewPort;
	RasterizerState				m_RasterizerStateID;
public:
	bool	Create(UINT width, UINT height);
	bool	Begin();
	bool	End();
	bool    Release();
public:
	MRenderTarget();
	virtual ~MRenderTarget();
};

