#pragma once
#include "MSelect.h"

class MPushPull
{
private:
	M3DHeightMap* m_pHM;
	float	m_fRadius;
	float	m_fPower;
	bool	m_bReversal;
private:
	void	ModifyVertex(DWORD index, float distance);
public:
	bool	SetRadius(float radius);
	bool	SetPower(float power);
	bool	PushPull();
public:
	MPushPull();
	~MPushPull();
};

