#include "M3DObject.h"



M3DObject::M3DObject()
{
	m_pConstantBuffer = NULL;
	m_pParents = nullptr;

	ZeroMemory(&m_ConstantOBJ, sizeof(m_ConstantOBJ));
	D3DXMatrixIdentity(&m_ConstantOBJ.matWorld);

	m_LocalPosition = (D3DXVECTOR3(0, 0, 0));
	m_LocalRotation = (D3DXQUATERNION(0, 0, 0, 1));
	m_LocalScale = (D3DXVECTOR3(1, 1, 1));

	m_WorldPosition = m_LocalPosition;
	m_WorldRotation = m_LocalRotation;
	m_WorldScale = m_LocalScale;

	m_bIsInherityPosition = true;
	m_bIsInherityRotation = true;
	m_bIsInherityScale = true;

}


M3DObject::~M3DObject()
{
}

bool M3DObject::Init()
{
	return true;
}

bool M3DObject::Release()
{
	return true;
}

bool M3DObject::Frame()
{
	return true;
}

bool M3DObject::PreFrame()
{
	// 자신의 월드행렬 변환
	D3DXMatrixTransformation(&m_ConstantOBJ.matWorld, NULL, NULL, &m_LocalScale, NULL, &m_LocalRotation, &m_LocalPosition);
	if (m_pParents != nullptr)
	{
		// 부모가 NULL이 아닐경우
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

	D3DXMatrixTranspose(&m_ConstantOBJ.matWorld, &m_ConstantOBJ.matWorld);
	if (m_pConstantBuffer)
	{
		g_pImmediateContext->UpdateSubresource(m_pConstantBuffer, 0, 0, &m_ConstantOBJ, 0, 0);
	}
	return true;
}

void M3DObject::LinkParents(M3DObject* Parents)
{
	if (Parents == this) return;
	m_pParents = Parents;
	return;
	/////////////////////////
	if (Parents == this) return;
	PreFrame();
	UnLinkParents();
	m_pParents = Parents;
	m_pParents->PreFrame();

	D3DXMATRIX tempTM = m_pParents->m_ConstantOBJ.matWorld;
	D3DXMatrixTranspose(&tempTM, &tempTM);
	D3DXMatrixInverse(&tempTM, NULL, &tempTM);
	D3DXVec3TransformCoord(&m_LocalPosition, &m_LocalPosition, &tempTM);

	D3DXQUATERNION temp;
	D3DXQuaternionConjugate(&temp, &m_pParents->m_WorldRotation);
	m_LocalRotation *= temp;

	m_LocalScale.x /= m_pParents->m_WorldScale.x;
	m_LocalScale.y /= m_pParents->m_WorldScale.y;
	m_LocalScale.z /= m_pParents->m_WorldScale.z;
}

void M3DObject::UnLinkParents()
{
	m_pParents = nullptr;
	return;
	/////////////////////////
	if (m_pParents != nullptr)
	{
		D3DXMATRIX tempTM = m_pParents->m_ConstantOBJ.matWorld;
		D3DXMatrixTranspose(&tempTM, &tempTM);
		D3DXVec3TransformCoord(&m_LocalPosition, &m_LocalPosition, &tempTM);

		m_LocalRotation *= m_pParents->m_WorldRotation;

		m_LocalScale.x *= m_pParents->m_WorldScale.x;
		m_LocalScale.y *= m_pParents->m_WorldScale.y;
		m_LocalScale.z *= m_pParents->m_WorldScale.z;

		m_pParents = nullptr;
	}
}

bool M3DObject::Render()
{
	return true;
}

void M3DObject::ModifyLocalPosition(D3DXVECTOR3 data)
{
	m_LocalPosition += data;

}
void M3DObject::ModifyLocalRotation(D3DXQUATERNION data)
{
	m_LocalRotation *= data;
}
void M3DObject::ModifyLocalScale(D3DXVECTOR3 data)
{
	m_LocalScale += data;

	if (m_LocalScale.x < 0) m_LocalScale.x = 0;
	if (m_LocalScale.y < 0) m_LocalScale.y = 0;
	if (m_LocalScale.z < 0) m_LocalScale.z = 0;
}

void M3DObject::SetLocalPosition(D3DXVECTOR3 data)
{
	m_LocalPosition = data;
}
void M3DObject::SetLocalRotation(D3DXQUATERNION data)
{
	m_LocalRotation = data;
}
void M3DObject::SetLocalScale(D3DXVECTOR3 data)
{
	m_LocalScale = data;
	if (m_LocalScale.x < 0) m_LocalScale.x = 0;
	if (m_LocalScale.y < 0) m_LocalScale.y = 0;
	if (m_LocalScale.z < 0) m_LocalScale.z = 0;
}
void M3DObject::SetColor(D3DXVECTOR3 data)
{
	GetMatarial()->m_ConstantMatrial.ObjectColor = data;
}


M3DObject * M3DObject::GetParents()
{
	return m_pParents;
}

D3DXVECTOR3 M3DObject::GetLocalPosition()
{
	return m_LocalPosition;
}
D3DXQUATERNION M3DObject::GetLocalRotation()
{
	return m_LocalRotation;
}
D3DXVECTOR3 M3DObject::GetLocalScale()
{
	return m_LocalScale;
}

D3DXMATRIX M3DObject::GetPositionMatrix()
{
	D3DXMATRIX temp;
	D3DXMatrixTranslation(&temp, m_LocalPosition.x, m_LocalPosition.y, m_LocalPosition.z);
	return temp;
}

D3DXMATRIX M3DObject::GetRotationMatrix()
{
	D3DXMATRIX temp;
	D3DXMatrixRotationQuaternion(&temp, &m_LocalRotation);
	return temp;
}

D3DXMATRIX M3DObject::GetScaleMatrix()
{
	D3DXMATRIX temp;
	D3DXMatrixScaling(&temp, m_LocalScale.x, m_LocalScale.y, m_LocalScale.z);
	return temp;
}

CONSTANT_3DOBJ M3DObject::GetConstantOBJ()
{
	return m_ConstantOBJ;
}

bool M3DObject::CreateConstantBuffer()
{
	D3D11_BUFFER_DESC BufferDesc;
	HRESULT CreateBufferResult;
	D3D11_SUBRESOURCE_DATA SubresourceData;

	ZeroMemory(&BufferDesc, sizeof(D3D11_BUFFER_DESC));
	BufferDesc.ByteWidth = sizeof(CONSTANT_3DOBJ);
	BufferDesc.Usage = D3D11_USAGE_DEFAULT;
	BufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

	ZeroMemory(&SubresourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	SubresourceData.pSysMem = &m_ConstantOBJ;

	if (FAILED(CreateBufferResult = g_pDevice->CreateBuffer(&BufferDesc, &SubresourceData, &m_pConstantBuffer))) return false;
	return true;
}

bool M3DObject::IsHasParents()
{
	if (m_pParents != nullptr)
	{
		return true;
	}
	return false;
}