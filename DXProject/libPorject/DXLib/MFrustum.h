#pragma once
#include "MStd.h"
#include "MBoundingBox.h"

struct MPlane
{
	float fA, fB, fC, fD;
	bool CreatePlane(D3DXVECTOR3 v0, D3DXVECTOR3 v1, D3DXVECTOR3 v2);
	bool CreatePlane(D3DXVECTOR3 vNormal, D3DXVECTOR3 v1);
	void Normalize();
};

class MFrustum
{
public:
	MPlane			m_Plane[5];
	D3DXVECTOR3		m_vFrustum[5];
	D3DXMATRIX      m_matView;
	D3DXMATRIX      m_matProj;
	bool			CheckPlane(MPlane plane, MBoundingBox* box);
	bool			CheckOBB(MBoundingBox* box);
public:
	void			CreateFrustum(D3DXMATRIX& matView, D3DXMATRIX& matProj);
public:
	MFrustum();
	~MFrustum();
};

