#pragma once
#include "MCameraMgr.h"
#include "M3DObjectMgr.h"
#include "MRenderTargetMgr.h"
#include "GeometryShaderMgr.h"
#include "MDXWirte.h"

class MDevice : public MSingleton<MDevice>
{
	friend class MSingleton<MDevice>;
public:
	vector<MDXWirte*>			m_dxWriteList;
public:
	D3D_DRIVER_TYPE				m_DirverType;			//// 디바이스타입 <하드웨어가속, 워프, 레퍼런스 등등>
	DXGI_SWAP_CHAIN_DESC		m_SwapChainDesc;		//// 스왑체인 정보
	D3D_FEATURE_LEVEL			m_FeatureLevel;			//// 버전정보
	IDXGISwapChain*				m_pSwapChain;			//// 더블 버퍼링을 위한 스왑체인
	ID3D11Device*				m_pDevice;				//// 디바이스 객체
	ID3D11DeviceContext*		m_pImmediateContext;	//// 디바이스 컨텍스트 객체
	ID3D11RenderTargetView*		m_pRenderTargetView;
	ID3D11DepthStencilView*		m_pDepthStencilView;
	D3D11_VIEWPORT				m_ViewPort;
	IDXGIFactory*				m_pGIFactory;			//// 그래픽인터페이스 팩토리
	RasterizerState				m_RasterizerStateID;
public:
	bool						ResizeSwapChain(UINT width, UINT height);
public:
	bool						CreateDevice();
	bool						CreateGIFactory();
	bool						CreateSwapChain();
	bool						SetRenderTargetView();
	bool						SetViewPort();
public:
	bool						Init();
	bool						Frame();
	bool						PreRender();
	bool						Render();
	bool						PostRender();
	bool						Release();
private:
	MDevice();
public:
	virtual ~MDevice();
};

#define I_Device MDevice::GetInstance()