#include "MUnit.h"

bool MUnit::SetRCVertex(vector<MVERTEX>& mv, vector<SKINDATA>& ms)
{
	if (m_OldData.size() == 0)
	{
		return false;
	}
	for (auto temp : m_OldData)
	{
		MVERTEX newv;
		newv.p = temp.p;
		newv.n = temp.n;
		newv.t = temp.t;
		newv.tv = temp.tv;

		SKINDATA news;
		ZeroMemory(&news, sizeof(SKINDATA));
		news.m_NumWeight = temp.bp.m_NumWeight;
		for (int i = 0; i < news.m_NumWeight; i++)
		{
			news.m_Weight[i] = temp.bp.m_Weight[i];
			news.m_ID[i] = m_Skeleton->NumofBone(temp.bp.m_ID[i]);
		}
		mv.push_back(newv);
		ms.push_back(news);
	}

	m_OldData.clear();
	return true;
}

bool MUnit::Init()
{
	I_MaterialMgr[MaterialID]->m_VertexShaderID = VSCHARACTER;
	return true;
}

bool MUnit::Frame()
{
	//PreFrame();
	return true;
}

bool MUnit::Render()
{
	I_MaterialMgr[MaterialID]->Render();

	UINT stride = sizeof(MVERTEX);		//// 보폭(카운트할 사이즈 크기)
	UINT offset = 0;
	g_pImmediateContext->IASetVertexBuffers(0, 1, &I_MeshMgr[ObjID]->m_pVertexBuffer, &stride, &offset);
	stride = sizeof(SKINDATA);
	g_pImmediateContext->IASetVertexBuffers(1, 1, &m_pSkinBuffer, &stride, &offset);
	g_pImmediateContext->IASetIndexBuffer(I_MeshMgr[ObjID]->m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	g_pImmediateContext->VSSetConstantBuffers(2, 1, &m_pConstantBuffer);
	g_pImmediateContext->VSSetConstantBuffers(3, 1, &m_Skeleton->m_pBoneBuffer);
	g_pImmediateContext->VSSetConstantBuffers(4, 1, &m_Skeleton->m_pZeroBuffer);
	/////////////////////////////////////////////////////////////////////////////////////
	g_pImmediateContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	g_pImmediateContext->DrawIndexed(I_MeshMgr[ObjID]->m_iIndexCount, 0, 0);
	return true;
}

bool MUnit::Release()
{
	SAFE_RELEASE(m_pConstantBuffer);
	return true;
}

MUnit::MUnit()
{

}


MUnit::~MUnit()
{
}
