#include "MFreeCamera.h"



MFreeCamera::MFreeCamera()
{
}


MFreeCamera::~MFreeCamera()
{
}

bool MFreeCamera::Init()
{
	D3DXMatrixIdentity(&m_matWorld);
	//// 카메라의 기본값 ////
	m_LookAt = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	m_Up = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	D3DXMatrixLookAtLH(&m_matView, &GetLocalPosition(), &m_LookAt, &m_Up);

	//// FOV 설정 ////
	D3DXMatrixPerspectiveFovLH(&m_matProj, D3DX_PI / 4, (float)g_rtWindowClient.right / (float)g_rtWindowClient.bottom, 1.0f, MAXDISTANCE);

	D3DXMatrixTranspose(&m_matWorld, &m_matWorld);
	D3DXMatrixTranspose(&m_matView, &m_matView);
	D3DXMatrixTranspose(&m_matProj, &m_matProj);



	CreateConstantBuffer();
	return true;
}

bool MFreeCamera::Frame()
{
	D3DXMatrixLookAtLH(&m_matView, &GetLocalPosition(), &(GetLocalPosition() + m_LookAt), &m_Up);
	D3DXMatrixTranspose(&m_matView, &m_matView);

	m_WorldPosition = m_LocalPosition;

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

void MFreeCamera::ModifyRotation(D3DXQUATERNION data)
{
	D3DXQUATERNION temp(0, 0, 0, 1);
	yew += data.x;
	pitch += data.y;
	if (pitch > 1.57)
	{
		pitch = 1.57;
	}
	else if(pitch < -1.57)
	{
		pitch = -1.57;
	}
	D3DXQuaternionRotationYawPitchRoll(&temp, yew, pitch, data.z);
	D3DXMATRIX mat;
	D3DXMatrixIdentity(&mat);
	D3DXMatrixRotationQuaternion(&mat, &temp);
	m_LookAt.x = 0;
	m_LookAt.y = 0;
	m_LookAt.z = 1;
	D3DXVec3TransformCoord(&m_LookAt, &m_LookAt, &mat);

}
void MFreeCamera::SetRotation(D3DXQUATERNION data)
{
	m_LocalRotation = data;
}