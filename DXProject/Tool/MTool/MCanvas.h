#pragma once
#include "MSelect.h"
class MCanvas
{
private:
	ID3D11Texture2D*			m_pCanvasTexture;
	ID3D11ShaderResourceView*	m_pShaderResourceView;	////
	ID3D11RenderTargetView*		m_pRenderTargetView;	//// 랜더타켓 뷰 = 오프스크린
	D3D11_VIEWPORT				m_ViewPort;				//// 메인스크린의 속성값
	UINT						m_iViewports;
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
	UINT						m_iRadius;			//지름
	D3DXVECTOR2					m_TextureSize;
	DWORD						m_BrushID;
	float						m_Channel;
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
	bool						Render();
	bool						Release();

	bool						Brushing();
public:
	MCanvas();
	~MCanvas();
};

