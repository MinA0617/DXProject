#include "M3DBone.h"



M3DBone::M3DBone()
{
	I_MaterialMgr[MaterialID]->m_PixelShaderID = PSSPLINE;
	m_VertexShaderID = VSSPLINE;
	m_BoxList.resize(0);
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
	m_LocalScale = data;
	//if (m_LocalScale.x < 0) m_LocalScale.x = 0;
	//if (m_LocalScale.y < 0) m_LocalScale.y = 0;
	//if (m_LocalScale.z < 0) m_LocalScale.z = 0;
}

bool M3DBone::SetZero()
{
	Frame();
	D3DXMatrixTransformation(&ZeroMatrix, NULL, NULL, &m_LocalScale, NULL, &m_LocalRotation, &m_LocalPosition);
	if (m_pParents != nullptr)
	{
		D3DXMATRIX ParentMat;
		ParentMat = m_pParents->m_ConstantOBJ.matWorld;
		D3DXMatrixTranspose(&ParentMat, &ParentMat);
		ZeroMatrix = ZeroMatrix * ParentMat;
	}
	D3DXMatrixInverse(&ZeroMatrix, NULL, &ZeroMatrix);
	D3DXMatrixTranspose(&ZeroMatrix, &ZeroMatrix);
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

	D3DXMatrixTransformation(&m_ConstantOBJ.matWorld, NULL, NULL, &m_LocalScale, NULL, &m_LocalRotation, &m_LocalPosition);

	if (m_pParents != nullptr)
	{
		D3DXMATRIX ParentMat;
		ParentMat = m_pParents->m_ConstantOBJ.matWorld;
		D3DXMatrixTranspose(&ParentMat, &ParentMat);
		m_ConstantOBJ.matWorld = m_ConstantOBJ.matWorld * ParentMat;

		D3DXMatrixDecompose(&m_WorldScale, &m_WorldRotation, &m_WorldPosition, &m_ConstantOBJ.matWorld);
	}
	else
	{
		m_WorldPosition = m_LocalPosition;
		m_WorldRotation = m_LocalRotation;
		m_WorldScale = m_LocalScale;
	}

	//UpdateBox();

	D3DXMatrixTranspose(&m_ConstantOBJ.matWorld, &m_ConstantOBJ.matWorld);

	if (m_pConstantBuffer)
	{
		g_pImmediateContext->UpdateSubresource(m_pConstantBuffer, 0, 0, &m_ConstantOBJ, 0, 0);
	}

	// -------------

	// -------------
	return true;
}

bool M3DBone::Render()
{
	//--------------------------------------------------------
	// 디버그용 랜더
	//--------------------------------------------------------
#if defined(DEBUG) || defined(_DEBUG)
	if (0)
	{
		if (g_dwIC_Point == 0)
			return false;
		I_MaterialMgr[MaterialID]->Render();

		UINT stride = sizeof(SPLINE_VERTEX);		//// 보폭(카운트할 사이즈 크기)
		UINT offset = 0;
		g_pImmediateContext->IASetVertexBuffers(0, 1, &g_pVB_Point, &stride, &offset);
		g_pImmediateContext->IASetIndexBuffer(g_pIB_Point, DXGI_FORMAT_R32_UINT, 0);
		g_pImmediateContext->VSSetConstantBuffers(1, 1, &m_pConstantBuffer);
		/////////////////////////////////////////////////////////////////////////////////////
		/////////////////////////////////////////////////////////////////////////////////////
		g_pImmediateContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINELIST);

		g_pImmediateContext->IASetInputLayout(I_VertexShaderMgr.m_LOList[m_VertexShaderID]);		// 레이아웃 셋
		g_pImmediateContext->VSSetShader(I_VertexShaderMgr.m_VSList[m_VertexShaderID], NULL, 0);

		g_pImmediateContext->DrawIndexed(g_dwIC_Point, 0, 0);
	}
		for (auto data : m_pChildList)
		{
			data->Render();
		}
	//if (m_BoxList != nullptr) m_BoxList->Render();
#endif
	return true;
}

bool M3DBone::UpdateKey(float time)
{
	if (m_TempKeyData.isUse())
	{
		if (m_TempKeyData.m_vPositionKeyList.size() != 0)
		{
			m_LocalPosition = m_TempKeyData.GetCurPosition(time);
		}
		if (m_TempKeyData.m_vRotationKeyList.size() != 0)
		{
			m_LocalRotation = m_TempKeyData.GetCurRotation(time);
		}
		if (m_TempKeyData.m_vScaleKeyList.size() != 0)
		{
			m_LocalScale = m_TempKeyData.GetCurScale(time);
		}
	}
	else
	{
		if (m_KeyData->m_vPositionKeyList.size() != 0)
		{
			m_LocalPosition = m_KeyData->GetCurPosition(time);
		}
		if (m_KeyData->m_vRotationKeyList.size() != 0)
		{
			m_LocalRotation = m_KeyData->GetCurRotation(time);
		}
		if (m_KeyData->m_vScaleKeyList.size() != 0)
		{
			m_LocalScale = m_KeyData->GetCurScale(time);
		}
	}
	return true;
}