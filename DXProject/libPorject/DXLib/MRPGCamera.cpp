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

	m_LookAt = m_pTarget->GetLocalPosition();

	D3DXMatrixLookAtLH(&m_matView, &m_LocalPosition, &m_LookAt, &m_Up);

	CameraFrame();
	return true;
}