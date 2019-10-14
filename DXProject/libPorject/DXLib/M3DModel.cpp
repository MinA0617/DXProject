#include "M3DModel.h"



M3DModel::M3DModel()
{
}


M3DModel::~M3DModel()
{
}

bool M3DModel::Init()
{
	ObjID = 0;
	return true;
}

bool M3DModel::Release()
{
	SAFE_RELEASE(m_pConstantBuffer);
	return true;
}

bool M3DModel::Frame()
{
	PreFrame();
	return true;
}

bool M3DModel::Render()
{
	///// 스테이트 매니져로 블랜드셋팅을 받아오기 ////

	I_MaterialMgr[MaterialID]->Render();

	UINT stride = sizeof(MVERTEX);		//// 보폭(카운트할 사이즈 크기)
	UINT offset = 0;					//// 시작 위치
	g_pImmediateContext->IASetVertexBuffers(0, 1, &I_MeshMgr[ObjID]->m_pVertexBuffer, &stride, &offset);
	g_pImmediateContext->IASetIndexBuffer(I_MeshMgr[ObjID]->m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	g_pImmediateContext->VSSetConstantBuffers(2, 1, &m_pConstantBuffer);

	/////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////
	g_pImmediateContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	g_pImmediateContext->DrawIndexed(I_MeshMgr[ObjID]->m_iIndexCount, 0, 0);
	return true;
}
//bool M3DModel::Load(M_STR filename)
//{
//	ObjID = I_MeshMgr.Load(filename);
//	if (ObjID == NULL)
//	{
//		return false;
//	}
//	CreateConstantBuffer();
//
//	return true;
//}
//bool M3DModel::CreateObj(MMesh* data)
//{
//	ObjID = I_MeshMgr.Add(data);
//	if (ObjID == NULL)
//	{
//		return false;
//	}
//	CreateConstantBuffer();
//	return true;
//}

bool M3DModel::Load_MAP(M_STR filename, MAPTYPE type)
{
	return I_MaterialMgr[MaterialID]->Load(filename, type);
}