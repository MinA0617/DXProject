#include "MKeyData.h"



MKeyData::MKeyData()
{
}


MKeyData::~MKeyData()
{
}


D3DXVECTOR3 MKeyData::GetCurPosition(float time)
{
	if (m_vPositionKeyList.size() == 0) return D3DXVECTOR3(0, 0, 0);
	if (m_vPositionKeyList.size() == 1)	return m_vPositionKeyList[0].data;
	float maxtime = m_vPositionKeyList[m_vPositionKeyList.size() - 1].tick;
	if (time > maxtime)
	{
		return m_vPositionKeyList[m_vPositionKeyList.size() - 1].data;
	}
	for (int i = 0; i < m_vPositionKeyList.size(); i++)
	{
		if (m_vPositionKeyList[i + 1].tick >= time)
		{
			KEY_Position cur = m_vPositionKeyList[i];
			KEY_Position next = m_vPositionKeyList[i + 1];
			float length = next.tick - cur.tick;
			float curtime = time - cur.tick;
			float a = curtime / length;
			float x = (cur.data.x * (1 - a)) + (next.data.x * a);
			float y = (cur.data.y * (1 - a)) + (next.data.y * a);
			float z = (cur.data.z * (1 - a)) + (next.data.z * a);
			return D3DXVECTOR3(x, y, z);
		}
	}
	return D3DXVECTOR3(0, 0, 0);
}

D3DXQUATERNION MKeyData::GetCurRotation(float time)
{
	if (m_vRotationKeyList.size() == 0) return D3DXQUATERNION(1, 0, 0, 0);
	if (m_vRotationKeyList.size() == 1)	return m_vRotationKeyList[0].data;
	float maxtime = m_vRotationKeyList[m_vRotationKeyList.size() - 1].tick;
	if (time > maxtime)
	{
		return m_vRotationKeyList[m_vRotationKeyList.size() - 1].data;
	}
	D3DXQUATERNION result;
	D3DXQuaternionIdentity(&result);
	for (int i = 0; i < m_vRotationKeyList.size(); i++)
	{
		if (m_vRotationKeyList[i + 1].tick >= time)
		{
			KEY_Rotation cur = m_vRotationKeyList[i];
			KEY_Rotation next = m_vRotationKeyList[i + 1];
			float length = next.tick - cur.tick;
			float curtime = time - cur.tick;
			float t = (curtime / length);
			D3DXQuaternionSlerp(&result, &cur.data, &next.data, t);
			return result;
		}
	}
	return D3DXQUATERNION(1, 0, 0, 0);
}

D3DXVECTOR3 MKeyData::GetCurScale(float time)
{
	if (m_vScaleKeyList.size() == 0) return D3DXVECTOR3(1, 1, 1);
	if (m_vScaleKeyList.size() == 1) return m_vScaleKeyList[0].data;
	float maxtime = m_vScaleKeyList[m_vScaleKeyList.size() - 1].tick;
	if (time > maxtime)
	{
		return m_vPositionKeyList[m_vScaleKeyList.size() - 1].data;
	}
	for (int i = 0; i < m_vScaleKeyList.size(); i++)
	{
		if (m_vScaleKeyList[i + 1].tick >= time)
		{
			KEY_Scale cur = m_vScaleKeyList[i];
			KEY_Scale next = m_vScaleKeyList[i + 1];
			float length = next.tick - cur.tick;
			float curtime = time - cur.tick;
			float a = curtime / length;
			float x = (cur.data.x * (1 - a)) + (next.data.x * a);
			float y = (cur.data.y * (1 - a)) + (next.data.y * a);
			float z = (cur.data.z * (1 - a)) + (next.data.z * a);
			return D3DXVECTOR3(x, y, z);
		}
	}
	return D3DXVECTOR3(1, 1, 1);
}

float MKeyData::GetMaxTime()
{
	float posmax;
	float rotmax;
	float sclmax;
	if (m_vPositionKeyList.size() <= 1)
	{
		posmax = 0;
	}
	else
	{
		posmax = m_vPositionKeyList[m_vPositionKeyList.size() - 1].tick;
	}
	if (m_vRotationKeyList.size() <= 1)
	{
		rotmax = 0;
	}
	else
	{
		rotmax = m_vRotationKeyList[m_vRotationKeyList.size() - 1].tick;
	}
	if (m_vScaleKeyList.size() <= 1)
	{
		sclmax = 0;
	}
	else
	{
		sclmax = m_vScaleKeyList[m_vScaleKeyList.size() - 1].tick;
	}
	return max(max(posmax, rotmax), sclmax);
}

bool MKeyData::isUse()
{
	return m_vPositionKeyList.size() + m_vRotationKeyList.size() + m_vScaleKeyList.size();
}
