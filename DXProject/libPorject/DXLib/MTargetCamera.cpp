#include "MTargetCamera.h"



MTargetCamera::MTargetCamera()
{
	m_pTarget = nullptr;
	m_fDistance = 10.0;
}


MTargetCamera::~MTargetCamera()
{
}

bool MTargetCamera::Init()
{
	D3DXMatrixIdentity(&m_matWorld);
	//// 카메라의 기본값 ////
	if (m_pTarget != nullptr)
	{
		m_LookAt = m_pTarget->GetLocalPosition();
		m_LocalPosition = m_pTarget->GetLocalPosition() - D3DXVECTOR3(sin(m_LocalRotation.y) * sin(m_LocalRotation.x), cos(m_LocalRotation.x), cos(m_LocalRotation.y) * sin(m_LocalRotation.x)) * -m_fDistance;
	}
	else
	{
		m_LookAt = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}
	m_Up = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	D3DXMatrixLookAtLH(&m_matView, &m_LocalPosition, &m_LookAt, &m_Up);

	//// FOV 설정 ////
	D3DXMatrixPerspectiveFovLH(&m_matProj, D3DX_PI / 4, (float)g_rtWindowClient.right / (float)g_rtWindowClient.bottom, 1.0f, 10000.0f);

	D3DXMatrixTranspose(&m_matWorld, &m_matWorld);
	D3DXMatrixTranspose(&m_matView, &m_matView);
	D3DXMatrixTranspose(&m_matProj, &m_matProj);

	CreateConstantBuffer();
	return true;
}

bool MTargetCamera::Frame()
{
	m_LookAt = m_pTarget->GetLocalPosition();

	D3DXMatrixLookAtLH(&m_matView, &GetLocalPosition(), &(GetLocalPosition() + m_LookAt), &m_Up);
	D3DXMatrixTranspose(&m_matView, &m_matView);

	D3D11_MAPPED_SUBRESOURCE MappedResource;
	if (SUCCEEDED(g_pImmediateContext->Map(m_pConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedResource)))
	{
		VS_WORLD_BUFFER* data = (VS_WORLD_BUFFER*)MappedResource.pData;
		data->matView = m_matView;
		data->matProj = m_matProj;
		data->vEyePos = m_WorldPosition;
		data->vEyeDir = m_LookAt;
		data->fClinetSizeW = g_rtWindowClient.right;
		data->fClinetSizeH = g_rtWindowClient.bottom;
		g_pImmediateContext->Unmap(m_pConstantBuffer, 0);
	}
	return true;
}

void MTargetCamera::ModifyRotation(D3DXQUATERNION data)
{
	// 롤 금지
	//data.y += data.y * abs(m_LocalRotation.x - D3DX_PI / 2) * 3;
	//m_LocalRotation += data;
	// 롤 금지
	// 사원수 배우기 전까지만 사용
	//m_LocalRotation.z = 0;
	if (m_LocalRotation.x < 0.01)
	{
		m_LocalRotation.x = 0.01;
	}
	if (m_LocalRotation.x > D3DX_PI * 0.99)
	{
		m_LocalRotation.x = D3DX_PI * 0.99;
	}
	if (m_LocalRotation.y > D3DX_PI * 2)
	{
		m_LocalRotation.y -= D3DX_PI * 2;
	}
	//At = D3DXVECTOR3(sin(m_WorldRotation.y), 0, cos(m_WorldRotation.y));
	//At = D3DXVECTOR3(0, cos(m_WorldRotation.x), -sin(m_WorldRotation.x));


	m_LocalPosition = m_pTarget->GetLocalPosition() - D3DXVECTOR3(sin(m_LocalRotation.y) * sin(m_LocalRotation.x), cos(m_LocalRotation.x), cos(m_LocalRotation.y) * sin(m_LocalRotation.x)) * -m_fDistance;
	//D3DXVECTOR3 Right;
	//D3DXVec3Cross(&Right, &D3DXVECTOR3(0, 0, 0), &At);
	//D3DXVec3Cross(&Up, &Right, &At);
}
void MTargetCamera::SetRotation(D3DXQUATERNION data)
{
	m_LocalRotation = data;
	m_LocalPosition = m_pTarget->GetLocalPosition() - D3DXVECTOR3(sin(m_LocalRotation.y) * sin(m_LocalRotation.x), cos(m_LocalRotation.x), cos(m_LocalRotation.y) * sin(m_LocalRotation.x)) * -m_fDistance;
}

void MTargetCamera::SetTarget(M3DObject* target)
{
	m_pTarget = target;
}