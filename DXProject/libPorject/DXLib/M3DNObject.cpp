#include "M3DNObject.h"
#include "M3DObjectMgr.h"

bool M3DNObject::Update()
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
	m_Box.Updata(&m_WorldPosition, &m_WorldRotation, &m_WorldScale);
	// --------------------------------------------------------------------------------------------------------
	D3DXMatrixTranspose(&m_WorldMatrix, &m_WorldMatrix);
	for (auto temp : m_pChildList)
	{
		temp->Update();
	}
	// ----------------------------------------------------------------------------------------------------------
	// 트리에서 업데이트
	if (I_3DObjectMgr.m_pTree)
	{
		if (!I_3DObjectMgr.m_pTree->CheckInstanceObject(this))
		{
			m_LocalPosition *= 0.99;
			m_LocalScale *= 0.99;
			Update();
		}
	}
	return true;
}

void M3DNObject::Link(M3DNObject* Parents)
{
	if (Parents == this) return;
	m_pParents = Parents;
	Parents->m_pChildList.push_back(this);
	return;
}

void M3DNObject::UnLink()
{
	for (list<M3DNObject*>::iterator data = m_pParents->m_pChildList.begin(); data != m_pParents->m_pChildList.end(); data++)
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

M3DNObject* M3DNObject::GetParents()
{
	return m_pParents;
}

void M3DNObject::ModifyPosition(D3DXVECTOR3 data)
{
	m_LocalPosition += data;
	Update();
}

void M3DNObject::ModifyRotation(D3DXQUATERNION data)
{
	m_LocalRotation *= data;
	Update();
}

void M3DNObject::ModifyScale(D3DXVECTOR3 data)
{
	m_LocalScale += data;
	if (m_LocalScale.x < 0) m_LocalScale.x = 0;
	if (m_LocalScale.y < 0) m_LocalScale.y = 0;
	if (m_LocalScale.z < 0) m_LocalScale.z = 0;
	Update();
}

void M3DNObject::SetPosition(D3DXVECTOR3 data)
{
	m_LocalPosition = data;
	Update();
}

void M3DNObject::SetRotation(D3DXQUATERNION data)
{
	m_LocalRotation = data;
	Update();
}

void M3DNObject::SetScale(D3DXVECTOR3 data)
{
	m_LocalScale = data;
	if (m_LocalScale.x < 0) m_LocalScale.x = 0;
	if (m_LocalScale.y < 0) m_LocalScale.y = 0;
	if (m_LocalScale.z < 0) m_LocalScale.z = 0;
	Update();
}

void M3DNObject::SetTransform(D3DXVECTOR3 pos, D3DXQUATERNION rot, D3DXVECTOR3 scl)
{
	m_LocalPosition = pos;
	m_LocalRotation = rot;
	m_LocalScale = scl;
	if (m_LocalScale.x < 0) m_LocalScale.x = 0;
	if (m_LocalScale.y < 0) m_LocalScale.y = 0;
	if (m_LocalScale.z < 0) m_LocalScale.z = 0;
	Update();
}

D3DXMATRIX M3DNObject::GetWorldMatrix()
{
	return m_WorldMatrix;
}

D3DXVECTOR3 M3DNObject::GetPosition()
{
	return m_WorldPosition;
}
D3DXQUATERNION M3DNObject::GetRotation()
{
	return m_WorldRotation;
}
D3DXVECTOR3 M3DNObject::GetScale()
{
	return m_WorldScale;
}

bool M3DNObject::Init()
{
#if defined(DEBUG) || defined(_DEBUG)
	m_Box.Init();
#endif
	return true;
}

bool M3DNObject::PreRender()
{
	return true;
}

bool M3DNObject::Release()
{
#if defined(DEBUG) || defined(_DEBUG)
	m_Box.Release();
#endif
	return true;
}

M3DNObject::M3DNObject()
{
	m_pParents = nullptr;

	m_LocalPosition = (D3DXVECTOR3(0, 0, 0));
	m_LocalRotation = (D3DXQUATERNION(0, 0, 0, 1));
	m_LocalScale = (D3DXVECTOR3(1, 1, 1));

	m_WorldPosition = m_LocalPosition;
	m_WorldRotation = m_LocalRotation;
	m_WorldScale = m_LocalScale;
}

M3DNObject::~M3DNObject()
{
}