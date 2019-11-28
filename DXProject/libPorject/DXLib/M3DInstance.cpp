#include "M3DInstance.h"
#include "M3DInstanceModel.h"
#include "M3DObjectMgr.h"


bool M3DInstance::Update()
{
	D3DXMatrixTransformation(&m_WorldMatrix, NULL, NULL, &m_LocalScale, NULL, &m_LocalRotation, &m_LocalPosition);
	if (m_pParents != nullptr)
	{
		// �θ� NULL�� �ƴҰ��
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
	// Ʈ������ ������Ʈ
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

void M3DInstance::SetTransform(D3DXVECTOR3 pos, D3DXQUATERNION rot, D3DXVECTOR3 scl)
{
	m_LocalPosition = pos;
	m_LocalRotation = rot;
	m_LocalScale = scl;
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