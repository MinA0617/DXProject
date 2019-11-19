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
	D3D_DRIVER_TYPE				m_DirverType;			//// ����̽�Ÿ�� <�ϵ�����, ����, ���۷��� ���>
	DXGI_SWAP_CHAIN_DESC		m_SwapChainDesc;		//// ����ü�� ����
	D3D_FEATURE_LEVEL			m_FeatureLevel;			//// ��������
	IDXGISwapChain*				m_pSwapChain;			//// ���� ���۸��� ���� ����ü��
	ID3D11Device*				m_pDevice;				//// ����̽� ��ü
	ID3D11DeviceContext*		m_pImmediateContext;	//// ����̽� ���ؽ�Ʈ ��ü
	ID3D11RenderTargetView*		m_pRenderTargetView;
	ID3D11DepthStencilView*		m_pDepthStencilView;
	D3D11_VIEWPORT				m_ViewPort;
	IDXGIFactory*				m_pGIFactory;			//// �׷����������̽� ���丮
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