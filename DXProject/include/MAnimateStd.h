#pragma once
#include "MStd.h"

struct KEY_Position
{
	float			tick;
	D3DXVECTOR3		data;
	KEY_Position(float t, float x, float y, float z)
	{
		tick = t;
		data.x = x;
		data.y = y;
		data.z = z;
	}

};

struct KEY_Rotation
{
	float			tick;
	D3DXQUATERNION	data;
	KEY_Rotation(float t, float x, float y, float z, float w)
	{
		tick = t;
		data.x = x;
		data.y = y;
		data.z = z;
		data.w = w;
	}

	KEY_Rotation(float t, D3DXQUATERNION  fdata)
	{
		tick = t;
		data = fdata;
	}

};

struct KEY_Scale
{
	float			tick;
	D3DXVECTOR3		data;
	KEY_Scale(float t, float x, float y, float z)
	{
		tick = t;
		data.x = x;
		data.y = y;
		data.z = z;
	}
};