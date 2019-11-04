#include "M3DModel.h"



M3DModel::M3DModel()
{
}


M3DModel::~M3DModel()
{
}
bool M3DModel::Init()
{
	CreateConstantBuffer();
	return true;
}
bool M3DModel::Release()
{
	SAFE_RELEASE(m_pConstantBuffer);
	for (auto data : m_pChildList)
	{
		data->Release();
	}
#if defined(DEBUG) || defined(_DEBUG)
	for (auto temp : m_BoxList)
	{
		temp->Release();
	}
#endif // DEBUG
	return true;
}

bool M3DModel::Frame()
{
	PreFrame();
	for (auto data : m_pChildList)
	{
		data->Frame();
	}
	return true;
}

bool M3DModel::Render()
{
	///// 스테이트 매니져로 블랜드셋팅을 받아오기 ////
	I_MaterialMgr[MaterialID]->Render();
	g_pImmediateContext->VSSetConstantBuffers(1, 1, &m_pConstantBuffer);
	m_pObj->Render();
	for (auto data : m_pChildList)
	{
		data->Render();
	}
#if defined(DEBUG) || defined(_DEBUG)
	for (auto temp : m_BoxList)
	{
		temp->Render();
	}
#endif // DEBUG
	return true;
}

bool M3DModel::Load_MAP(M_STR filename, MAPTYPE type)
{
	return I_MaterialMgr[MaterialID]->Load(filename, type);
}