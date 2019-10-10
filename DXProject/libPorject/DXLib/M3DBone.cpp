#include "M3DBone.h"



M3DBone::M3DBone()
{
	I_MaterialMgr[MaterialID]->m_VertexShaderID = VSSPLINE;
	I_MaterialMgr[MaterialID]->m_PixelShaderID = PSSPLINE;
}


M3DBone::~M3DBone()
{
}


void M3DBone::ModifyLocalPosition(D3DXVECTOR3 data)
{
	m_LocalPosition += data;
}

void M3DBone::ModifyLocalRotation(D3DXQUATERNION data)
{
	m_LocalRotation *= data;
}

void M3DBone::ModifyLocalScale(D3DXVECTOR3 data)
{
	m_LocalScale += data;
	if (m_LocalScale.x < 0) m_LocalScale.x = 0;
	if (m_LocalScale.y < 0) m_LocalScale.y = 0;
	if (m_LocalScale.z < 0) m_LocalScale.z = 0;
}

void M3DBone::SetLocalPosition(D3DXVECTOR3 data)
{
	m_LocalPosition = data;
}

void M3DBone::SetLocalRotation(D3DXQUATERNION data)
{
	m_LocalRotation = data;
}

void M3DBone::SetLocalScale(D3DXVECTOR3 data)
{
	//m_LocalScale = data;
	//if (m_LocalScale.x < 0) m_LocalScale.x = 0;
	//if (m_LocalScale.y < 0) m_LocalScale.y = 0;
	//if (m_LocalScale.z < 0) m_LocalScale.z = 0;
}

bool M3DBone::SetZero()
{
	D3DXMatrixTransformation(&ZeroMatrix, NULL, NULL, &m_LocalScale, NULL, &m_LocalRotation, &m_LocalPosition);
	D3DXMatrixInverse(&ZeroMatrix, NULL, &ZeroMatrix);
	return true;
}

bool M3DBone::Init()
{
#if defined(DEBUG) || defined(_DEBUG)
	CreateConstantBuffer();
#endif
	return true;
}

bool M3DBone::Frame()
{
	//PreFrame();
	D3DXMatrixTransformation(&m_ConstantOBJ.matWorld, NULL, NULL, &m_LocalScale, NULL, &m_LocalRotation, &m_LocalPosition);
	if (m_pParents != nullptr)
	{
		D3DXMATRIX ParentMat;
		ParentMat = m_pParents->m_ConstantOBJ.matWorld;
		D3DXMatrixTranspose(&ParentMat, &ParentMat);
		//m_ConstantOBJ.matWorld = m_ConstantOBJ.matWorld * ZeroMatrix;
		m_ConstantOBJ.matWorld = m_ConstantOBJ.matWorld * ParentMat;

		D3DXMatrixDecompose(&m_WorldScale, &m_WorldRotation, &m_WorldPosition, &m_ConstantOBJ.matWorld);
	}
	else
	{
		m_WorldPosition = m_LocalPosition;
		m_WorldRotation = m_LocalRotation;
		m_WorldScale = m_LocalScale;
	}

	D3DXMatrixTranspose(&m_ConstantOBJ.matWorld, &m_ConstantOBJ.matWorld);

	if (m_pConstantBuffer)
	{
		g_pImmediateContext->UpdateSubresource(m_pConstantBuffer, 0, 0, &m_ConstantOBJ, 0, 0);
	}
	return true;



	//D3DXMatrixTransformation(&m_ConstantOBJ.matWorld, NULL, NULL, &m_LocalScale, NULL, &m_LocalRotation, &m_LocalPosition);
	//D3DXMatrixTranspose(&m_ConstantOBJ.matWorld, &m_ConstantOBJ.matWorld);
	//if (m_pParents != nullptr)
	//{
	//	m_ConstantOBJ.matWorld = m_ConstantOBJ.matWorld * m_pParents->m_ConstantOBJ.matWorld;
	//	D3DXMatrixDecompose(&m_WorldScale, &m_WorldRotation, &m_WorldPosition, &m_ConstantOBJ.matWorld);
	//}
	//else
	//{
	//	m_WorldPosition = m_LocalPosition;
	//	m_WorldRotation = m_LocalRotation;
	//	m_WorldScale = m_LocalScale;
	//}


	//if (m_pConstantBuffer)
	//{
	//	g_pImmediateContext->UpdateSubresource(m_pConstantBuffer, 0, 0, &m_ConstantOBJ, 0, 0);
	//}
	//return true;
}

bool M3DBone::Render()
{
	//--------------------------------------------------------
	// 디버그용 랜더
	//--------------------------------------------------------
#if defined(DEBUG) || defined(_DEBUG)
	if (m_iIndexCount == 0)
		return false;
	I_MaterialMgr[MaterialID]->Render();

	UINT stride = sizeof(SPLINE_VERTEX);		//// 보폭(카운트할 사이즈 크기)
	UINT offset = 0;
	g_pImmediateContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);
	g_pImmediateContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	g_pImmediateContext->VSSetConstantBuffers(1, 1, &m_pConstantBuffer);
	/////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////
	g_pImmediateContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINELIST);
	g_pImmediateContext->DrawIndexed(m_iIndexCount, 0, 0);
#endif
	return true;
}