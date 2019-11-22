#pragma once
#include "MAnimateStd.h"

class MKeyData
{
public:
	vector<KEY_Position>	m_vPositionKeyList;
	vector<KEY_Rotation>	m_vRotationKeyList;
	vector<KEY_Scale>		m_vScaleKeyList;
	D3DXVECTOR3				GetCurPosition(float time);
	D3DXQUATERNION			GetCurRotation(float time);
	D3DXVECTOR3				GetCurScale(float time);
public:
	float					GetMaxTime();
public:
	bool isUse();
public:
	MKeyData();
	~MKeyData();
};

