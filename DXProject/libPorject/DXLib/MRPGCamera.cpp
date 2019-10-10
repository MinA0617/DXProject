#include "MRPGCamera.h"



MRPGCamera::MRPGCamera()
{
	m_bIsClick = false;
}


MRPGCamera::~MRPGCamera()
{

}

bool MRPGCamera::Frame()
{
	POINT pos;
	GetCursorPos(&pos);
	float x = 0;
	float y = 0;
	if (g_ActionInput.a_RightClick == KEY_PUSH)
	{
		m_bIsClick = true;
	}
	if (g_ActionInput.a_RightClick == KEY_UP)
	{
		m_bIsClick = false;
	}
	if (g_ActionInput.Q >= KEY_PUSH)
	{
		m_fDistance += 10;
	}
	if (g_ActionInput.E >= KEY_PUSH)
	{
		m_fDistance -= 10;
		if (m_fDistance < 0)
		{
			m_fDistance = 0;
		}
	}
	if (m_bIsClick)
	{
		x = (pos.x - m_beforeMousePos.x) * 0.001;
		y = (pos.y - m_beforeMousePos.y) * 0.001;
	}
	ModifyRotation(D3DXQUATERNION(-y, x, 0, 0));

	m_beforeMousePos = pos;

	m_LookAt = m_pTarget->GetPosition();

	D3DXMatrixLookAtLH(&m_matView, &m_LocalPosition, &m_LookAt, &m_Up);
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