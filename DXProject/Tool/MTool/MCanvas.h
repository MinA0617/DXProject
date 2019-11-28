#pragma once
#include "MSelect.h"
class MCanvas
{
private:
	ID3D11Texture2D*			m_pCanvasTexture;
	ID3D11ShaderResourceView*	m_pShaderResourceView;
	ID3D11RenderTargetView*		m_pRenderTargetView;
	//ID3D11Texture2D*			m_pBeforeTexture;
	//ID3D11ShaderResourceView*	m_pBeforeShaderResourceView;
	//ID3D11RenderTargetView*		m_pBeforeRenderTargetView;
	ID3D11Texture2D*			m_pPreviewTexture;
	ID3D11ShaderResourceView*	m_pPreviewShaderResourceView;
	ID3D11RenderTargetView*		m_pPreviewRenderTargetView;
	D3D11_VIEWPORT				m_ViewPort;
	UINT						m_iViewports;
	//UINT						m_iPreViewViewports;
	D3DXVECTOR3					m_center;
private:
	ID3D11RenderTargetView*		m_pOldRenderTargetView;
	ID3D11DepthStencilView*		m_pOldDepthStencilView;
	D3D11_VIEWPORT				m_OldViewPort;
	RasterizerState				m_RasterizerStateID;
private:
	ID3D11BlendState*			m_BS;
	ID3D11BlendState*			m_BSERASER;
	ID3D11PixelShader*			m_PS;
	ID3D11PixelShader*			m_ERASER;
	ID3D11VertexShader*			m_VS;
	ID3D11InputLayout*			m_LO;
	ID3D11Buffer*				m_pBuffer;
	ID3D11Buffer*				m_pConstantBuffer;
public:
	vector<DWORD>				m_BrushList;
	CONSTANT_2DOBJ				m_ConstantOBJ;
	UINT						m_iRadius;			//Áö¸§
	D3DXVECTOR2					m_TextureSize;
	DWORD						m_BrushID;
	float						m_Channel;
	bool						m_bIsRealDraw;
	bool						m_bIsEraser;
	float						m_fOpacity;
public:
	ID3D11Texture2D*			GetTexture();
	bool						LoadBrush(M_STR filepath);
	bool						Create(float xsize, float ysize);
	bool						Load(M_STR filepath);
	bool						Begin();
	bool						End();

	bool						Init();
	bool						Frame();
	bool						Draw();
	bool						Release();
public:
	MCanvas();
	~MCanvas();
};

