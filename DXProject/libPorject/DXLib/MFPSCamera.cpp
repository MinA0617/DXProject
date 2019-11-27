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

	CameraFrame();
	return true;
}