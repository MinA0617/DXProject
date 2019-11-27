#include "M3DInstance.h"
#include "M3DInstanceModel.h"
#include "M3DObjectMgr.h"


bool M3DInstance::Update()
{
	D3DXMatrixTransformation(&m_WorldMatrix, NULL, NULL, &m_LocalScale, NULL, &m_LocalRotation, &m_LocalPosition);
	if (m_pParents != nullptr)
	{
		// 부모가 NULL이 아닐경우
		D3DXMATRIX ParentMat = m_pParents->m_WorldMatrix;
		D3DXMatrixTranspose(&ParentMat, &ParentMat);
		m_WorldMatrix *= ParentMat;

		D3DXMatrixDecompose(&m_WorldScale, &m_WorldRotation, &m_WorldPosition, &m_WorldMatrix);
	}
	else
	{
		m_WorldPosition = m_LocalPosition;
		m_WorldRotation = m_LocalRotation;
		m_WorldScale = m_LocalScale;
	}
	// --- box update --------------------------------------------------------------------------------
	D3DXMATRIX boxmat = m_WorldMatrix;
	D3DXVec3TransformCoord(&m_Box.vCenter, &m_Box.vOldCenter, &boxmat);
	m_Box.fExtent[0] = m_Box.fOldExtent[0] * m_WorldScale.x;
	m_Box.fExtent[1] = m_Box.fOldExtent[1] * m_WorldScale.y;
	m_Box.fExtent[2] = m_Box.fOldExtent[2] * m_WorldScale.z;
	m_Box.vMin.x = m_Box.vCenter.x - m_Box.fExtent[0];
	m_Box.vMin.y = m_Box.vCenter.y - m_Box.fExtent[1];
	m_Box.vMin.z = m_Box.vCenter.z - m_Box.fExtent[2];
	m_Box.vMax.x = m_Box.vCenter.x + m_Box.fExtent[0];
	m_Box.vMax.y = m_Box.vCenter.y + m_Box.fExtent[1];
	m_Box.vMax.z = m_Box.vCenter.z + m_Box.fExtent[2];
	D3DXVECTOR3 vOldAxis[3];
	vOldAxis[0] = D3DXVECTOR3(1, 0, 0);
	vOldAxis[1] = D3DXVECTOR3(0, 1, 0);
	vOldAxis[2] = D3DXVECTOR3(0, 0, 1);
	boxmat._41 = 0;
	boxmat._42 = 0;
	boxmat._43 = 0;
	D3DXVec3TransformCoord(&m_Box.vAxis[0], &vOldAxis[0], &boxmat);
	D3DXVec3TransformCoord(&m_Box.vAxis[1], &vOldAxis[1], &boxmat);
	D3DXVec3TransformCoord(&m_Box.vAxis[2], &vOldAxis[2], &boxmat);
	D3DXVec3Normalize(&m_Box.vAxis[0], &m_Box.vAxis[0]);
	D3DXVec3Normalize(&m_Box.vAxis[1], &m_Box.vAxis[1]);
	D3DXVec3Normalize(&m_Box.vAxis[2], &m_Box.vAxis[2]);
#if defined(DEBUG) || defined(_DEBUG)
	if (m_Box.m_pConstantBuffer)
	{
		D3DXVECTOR3 scl;
		scl.x = m_Box.fExtent[0];
		scl.y = m_Box.fExtent[1];
		scl.z = m_Box.fExtent[2];
		D3DXMatrixTransformation(&boxmat, NULL, NULL, &scl, NULL, &m_WorldRotation, &m_Box.vCenter);
		D3DXMatrixTranspose(&boxmat, &boxmat);
		g_pImmediateContext->UpdateSubresource(m_Box.m_pConstantBuffer, 0, 0, &boxmat, 0, 0);
	}
#endif
	// --------------------------------------------------------------------------------------------------------
	D3DXMatrixTranspose(&m_WorldMatrix, &m_WorldMatrix);
	for (auto temp : m_pChildList)
	{
		temp->Update();
	}
	// ----------------------------------------------------------------------------------------------------------
	// 트리에서 업데이트
	if (I_3DObjectMgr.m_pTree)I_3DObjectMgr.m_pTree->CheckInstanceObject(this);
	return true;
}

void M3DInstance::Link(M3DInstance* Parents)
{
	if (Parents == this) return;
	m_pParents = Parents;
	Parents->m_pChildList.push_back(this);
	return;
}

void M3DInstance::UnLink()
{
	for (list<M3DInstance*>::iterator data = m_pParents->m_pChildList.begin(); data != m_pParents->m_pChildList.end(); data++)
	{
		if ((*data) == this)
		{
			m_pParents->m_pChildList.erase(data);
			m_pParents = nullptr;
			return;
		}
	}
	return;
}

M3DInstance* M3DInstance::GetParents()
{
	return m_pParents;
}

void M3DInstance::ModifyPosition(D3DXVECTOR3 data)
{
	m_LocalPosition += data;
	Update();
}

void M3DInstance::ModifyRotation(D3DXQUATERNION data)
{
	m_LocalRotation *= data;
	Update();
}

void M3DInstance::ModifyScale(D3DXVECTOR3 data)
{
	m_LocalScale += data;
	if (m_LocalScale.x < 0) m_LocalScale.x = 0;
	if (m_LocalScale.y < 0) m_LocalScale.y = 0;
	if (m_LocalScale.z < 0) m_LocalScale.z = 0;
	Update();
}

void M3DInstance::SetPosition(D3DXVECTOR3 data)
{
	m_LocalPosition = data;
	Update();
}

void M3DInstance::SetRotation(D3DXQUATERNION data)
{
	m_LocalRotation = data;
	Update();
}

void M3DInstance::SetScale(D3DXVECTOR3 data)
{
	m_LocalScale = data;
	if (m_LocalScale.x < 0) m_LocalScale.x = 0;
	if (m_LocalScale.y < 0) m_LocalScale.y = 0;
	if (m_LocalScale.z < 0) m_LocalScale.z = 0;
	Update();
}

D3DXVECTOR3 M3DInstance::GetPosition()
{
	return m_WorldPosition;
}
D3DXQUATERNION M3DInstance::GetRotation()
{
	return m_WorldRotation;
}
D3DXVECTOR3 M3DInstance::GetScale()
{
	return m_WorldScale;
}

bool M3DInstance::Init()
{
#if defined(DEBUG) || defined(_DEBUG)
	m_Box.Init();
#endif
	return true;
}

bool M3DInstance::PreRender()
{
	m_pModel->AddRenderObject(this);
	return true;
}

bool M3DInstance::Release()
{
#if defined(DEBUG) || defined(_DEBUG)
	m_Box.Release();
#endif
	return true;
}

M3DInstance::M3DInstance(M3DInstanceModel * target)
{
	m_pModel = target;

	m_pParents = nullptr;

	m_LocalPosition = (D3DXVECTOR3(0, 0, 0));
	m_LocalRotation = (D3DXQUATERNION(0, 0, 0, 1));
	m_LocalScale = (D3DXVECTOR3(1, 1, 1));

	m_WorldPosition = m_LocalPosition;
	m_WorldRotation = m_LocalRotation;
	m_WorldScale = m_LocalScale;
}

M3DInstance::~M3DInstance()
{
}