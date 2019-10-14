#include "MFPSCamera.h"



MFPSCamera::MFPSCamera()
{
	isCenterMouse = false;
}


MFPSCamera::~MFPSCamera()
{
}


bool MFPSCamera::Frame()
{
	if (isRotition)
	{
		float x = g_MousePos.x * 0.001;
		float y = g_MousePos.y * -0.001;

		POINT pos;
		GetCursorPos(&pos);

		pos.x = pos.x - g_MousePos.x;
		pos.y = pos.y + g_MousePos.y;

		SetCursorPos(pos.x, pos.y);
		if (!isCenterMouse)
		{
			isCenterMouse = !isCenterMouse;
		}
		else
		{
			D3DXQUATERNION temp(x, y, 0, 0);
			ModifyRotation(temp);
		}
	}
	else
	{
		isCenterMouse = false;
	}

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