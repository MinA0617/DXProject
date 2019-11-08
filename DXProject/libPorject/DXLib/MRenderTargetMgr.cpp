#include "MRenderTargetMgr.h"
#include "M3DObjectMgr.h"

int MRenderTargetMgr::CreateNewRenderTarget(UINT width, UINT heigth)
{
	MRenderTarget* NewRT = new MRenderTarget;
	if (!NewRT->Create(width, heigth))
	{
		return -1;
	}
	m_RTList.push_back(NewRT);
	return m_RTList.size() - 1;
}

int MRenderTargetMgr::CreateNewScreen(UINT width, UINT heigth)
{
	M2DScreen* NewSC = new M2DScreen;
	NewSC->Init();
	if (!NewSC->Create(width, heigth))
	{
		return -1;
	}
	m_SCList.push_back(NewSC);

	return m_SCList.size() - 1;
}

bool MRenderTargetMgr::Init()
{
	CreateNewRenderTarget(g_rtWindowClient.right, g_rtWindowClient.bottom);
	CreateNewScreen(g_rtWindowClient.right, g_rtWindowClient.bottom);
	return true;
}

bool MRenderTargetMgr::Frame()
{
	I_3DObjectMgr.Frame();
	return false;
}

bool MRenderTargetMgr::PreRender()
{
	g_pImmediateContext->ClearRenderTargetView(g_pRenderTargetView, D3DXVECTOR4(0.5, 0.5, 0.5, 1));
	g_pImmediateContext->ClearDepthStencilView(g_pDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	return false;
}

bool MRenderTargetMgr::Render()
{
	for (int i = 0; i < m_RTList.size(); i++)
	{
		m_RTList[i]->Begin();
		I_3DObjectMgr.Render();
		m_RTList[i]->End();
	}
	return false;
}

bool MRenderTargetMgr::PostRender()
{
	// 메인스크린으로 옮기기
	m_SCList[0]->Frame();
	g_pImmediateContext->OMSetRenderTargets(1, &g_pRenderTargetView, g_pDepthStencilView);
	g_pImmediateContext->RSSetViewports(1, &g_ViewPort);
	g_pImmediateContext->PSSetShaderResources(0, 1, &m_RTList[0]->m_pShaderResourceView);
	m_SCList[0]->Render();
	return true;
}

bool MRenderTargetMgr::Release()
{
	for (auto data : m_RTList)
	{
		data->Release();
		delete data;
	}
	m_RTList.clear();
	return true;
}

MRenderTargetMgr::MRenderTargetMgr()
{
}


MRenderTargetMgr::~MRenderTargetMgr()
{
}
