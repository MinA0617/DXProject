#include "MSkinModel.h"
#include "MParser.h"

void MSkinModel::LinkParents(M3DObject * Parents)
{
	return;
}

void MSkinModel::UnLinkParents()
{
	return;
}

bool MSkinModel::Init()
{
	return true;
}

bool MSkinModel::Frame()
{
	//PreFrame();
	return true;
}

bool MSkinModel::Render()
{
	I_MaterialMgr[MaterialID]->Render();
	g_pImmediateContext->VSSetConstantBuffers(1, 1, &m_pConstantBuffer);
	g_pImmediateContext->VSSetConstantBuffers(3, 1, &m_Skeleton->m_pBoneBuffer);
	g_pImmediateContext->VSSetConstantBuffers(4, 1, &m_Skeleton->m_pZeroBuffer);
	/////////////////////////////////////////////////////////////////////////////////////
	m_pObj->Render();
	return true;
}

bool MSkinModel::Release()
{
	SAFE_RELEASE(m_pConstantBuffer);
	return true;
}

MSkinModel::MSkinModel()
{

}


MSkinModel::~MSkinModel()
{
}

