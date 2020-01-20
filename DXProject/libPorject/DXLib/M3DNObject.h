#pragma once
#include "MStd.h"
#include "MBoundingBox.h"

class MTree;
class M3DNObject
{
	friend class MTree;
public:
	MBoundingBox			m_Box;
protected:
	// --------------------------------------------------
	D3DXMATRIX				m_WorldMatrix;
	// --------------------------------------------------
	D3DXVECTOR3				m_WorldPosition;
	D3DXQUATERNION			m_WorldRotation;
	D3DXVECTOR3				m_WorldScale;
	// --------------------------------------------------
	D3DXVECTOR3				m_LocalPosition;
	D3DXQUATERNION			m_LocalRotation;
	D3DXVECTOR3				m_LocalScale;
protected:
	M3DNObject*				m_pParents;
	list<M3DNObject*>		m_pChildList;
public:
	void					Link(M3DNObject* Parents);
	void					UnLink();
	M3DNObject*				GetParents();
public:
	bool					Update();
	void					ModifyPosition(D3DXVECTOR3 data);
	void					ModifyRotation(D3DXQUATERNION data);
	void					ModifyScale(D3DXVECTOR3 data);
	void					SetPosition(D3DXVECTOR3 data);
	void					SetRotation(D3DXQUATERNION data);
	void					SetScale(D3DXVECTOR3 data);
	void					SetTransform(D3DXVECTOR3 pos, D3DXQUATERNION rot, D3DXVECTOR3 scl);
	D3DXMATRIX				GetWorldMatrix();
	D3DXVECTOR3				GetPosition();
	D3DXQUATERNION			GetRotation();
	D3DXVECTOR3				GetScale();
public:
	bool					Init();
	bool					PreRender();
	bool					Release();
public:
	M3DNObject();
	virtual ~M3DNObject();
};
