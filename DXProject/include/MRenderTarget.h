#pragma once
#include "MStd.h"
#include "MDxState.h"
#include "M2DScreen.h"

class MRenderTarget
{
public:
	ID3D11Texture2D*			m_pTexture;				//// ����Ÿ�� �ؽ���
	ID3D11ShaderResourceView*	m_pShaderResourceView;	////
	ID3D11RenderTargetView*		m_pRenderTargetView;	//// ����Ÿ�� �� = ������ũ��
	ID3D11DepthStencilView*		m_pDepthStencilView;	//// ���ĽǺ�
	ID3D11Texture2D*			m_pDSTexture;			////
	D3D11_VIEWPORT				m_ViewPort;				//// ���ν�ũ���� �Ӽ���
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

