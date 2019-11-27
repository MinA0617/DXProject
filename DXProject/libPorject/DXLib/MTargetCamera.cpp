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
	//// ī�޶��� �⺻�� ////
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

	//// FOV ���� ////
	D3DXMatrixPerspectiveFovLH(&m_matProj, D3DX_PI / 4, (float)g_rtWindowClient.right / (float)g_rtWindowClient.bottom, 1.0f, MAXDISTANCE);

	D3DXMatrixTranspose(&m_matWorld, &m_matWorld);

	CreateConstantBuffer();
	return true;
}

bool MTargetCamera::Frame()
{
	m_LookAt = m_pTarget->GetLocalPosition();

	D3DXMatrixLookAtLH(&m_matView, &GetLocalPosition(), &(GetLocalPosition() + m_LookAt), &m_Up);

	CameraFrame();
	return true;
}

void MTargetCamera::ModifyRotation(D3DXQUATERNION data)
{
	// �� ����
	//data.y += data.y * abs(m_LocalRotation.x - D3DX_PI / 2) * 3;
	//m_LocalRotation += data;
	// �� ����
	// ����� ���� �������� ���
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