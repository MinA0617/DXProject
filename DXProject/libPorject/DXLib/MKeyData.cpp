#include "MKeyData.h"



MKeyData::MKeyData()
{
}


MKeyData::~MKeyData()
{
}


D3DXVECTOR3 MKeyData::GetCurPosition(float &time)
{
	//g_fSeoundPerFrame
	if (m_vPositionKeyList.size() == 1)
	{
		return m_vPositionKeyList[0].data;
	}
	float maxtime = m_vPositionKeyList[m_vPositionKeyList.size() - 1].tick;
	while (time > maxtime)
	{
		time -= maxtime;
	}
	for (int i = 1; i < m_vPositionKeyList.size(); i++)
	{
		if (m_vPositionKeyList[i].tick >= time)
		{
			D3DXVECTOR3 next = m_vPositionKeyList[i].data;
			D3DXVECTOR3 cur = m_vPositionKeyList[i - 1].data;
			float length = m_vPositionKeyList[i].tick - m_vPositionKeyList[i - 1].tick;
			float curtime = time - m_vPositionKeyList[i - 1].tick;
			float a = curtime / length;
			float x = (cur.x * (1 - a)) + (next.x * a);
			float y = (cur.y * (1 - a)) + (next.y * a);
			float z = (cur.z * (1 - a)) + (next.z * a);
			return D3DXVECTOR3(x, y, z);
		}
	}
	return D3DXVECTOR3(0, 0, 0);
}

D3DXQUATERNION MKeyData::GetCurRotation(float &time)
{
	if (m_vRotationKeyList.size() == 1)
	{
		return m_vRotationKeyList[0].data;
	}
	float maxtime = m_vRotationKeyList[m_vRotationKeyList.size() - 1].tick;
	while (time > maxtime)
	{
		time -= maxtime;
	}
	D3DXQUATERNION result;
	D3DXQuaternionIdentity(&result);
	for (int i = 1; i < m_vRotationKeyList.size(); i++)
	{
		if (m_vRotationKeyList[i].tick >= time)
		{
			KEY_Rotation next = m_vRotationKeyList[i];
			KEY_Rotation Slerp = m_vRotationKeyList[i - 1];
			float length = next.tick - Slerp.tick;
			float curtime = time - Slerp.tick;
			float t = (curtime / length);
			D3DXQuaternionSlerp(&result, &Slerp.data, &next.data, t);
			return result;
		}
	}
	return D3DXQUATERNION(1, 0, 0, 0);
}

D3DXVECTOR3 MKeyData::GetCurScale(float &time)
{
	if (m_vScaleKeyList.size() == 1)
	{
		return m_vScaleKeyList[0].data;

	}
	float maxtime = m_vScaleKeyList[m_vScaleKeyList.size() - 1].tick;
	while (time > maxtime)
	{
		time -= maxtime;
	}
	for (int i = 1; i < m_vScaleKeyList.size(); i++)
	{
		if (m_vScaleKeyList[i].tick >= time)
		{
			D3DXVECTOR3 next = m_vScaleKeyList[i].data;
			D3DXVECTOR3 cur = m_vScaleKeyList[i - 1].data;
			float length = m_vScaleKeyList[i].tick - m_vScaleKeyList[i - 1].tick;
			float curtime = time - m_vScaleKeyList[i - 1].tick;
			float a = curtime / length;
			float x = (cur.x * (1 - a)) + (next.x * a);
			float y = (cur.y * (1 - a)) + (next.y * a);
			float z = (cur.z * (1 - a)) + (next.z * a);
			return D3DXVECTOR3(x, y, z);
		}
	}
	return D3DXVECTOR3(1, 1, 1);
}

bool MKeyData::isUse()
{
	return m_vPositionKeyList.size() + m_vRotationKeyList.size() + m_vScaleKeyList.size();
}
