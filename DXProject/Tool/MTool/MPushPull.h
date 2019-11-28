#pragma once
#include "MSelect.h"
#include "M3DSpline.h"

class MPushPull
{
private:
	M3DSpline* m_pCircle;
	M3DHeightMap* m_pHM;
	float	m_fTimer;
	float	m_fRadius;
	float	m_fPower;
	set<DWORD> m_Facelist;
	D3DXVECTOR3 m_Center;
public:
	bool	m_bReversal;
private:
	void	ModifyVertex(DWORD index, float distance);
public:
	void	SetNonRender();
	bool	SetRadius(float radius);
	bool	SetPower(float power);
	bool	PushPull();
	bool	Update();
	bool	Init();
	bool	Release();
	bool	Frame();
public:
	MPushPull();
	~MPushPull();
};

