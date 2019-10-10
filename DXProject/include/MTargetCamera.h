#pragma once
#include "MCamera.h"

class MTargetCamera : public MCamera
{
	friend class MCameraMgr;
protected:
	M3DObject*			m_pTarget;
	float				m_fDistance;
public:
	void SetTarget(M3DObject* target);
	virtual void SetRotation(D3DXQUATERNION data);
	virtual void ModifyRotation(D3DXQUATERNION data);
public:
	virtual bool Init();
	virtual bool Frame();
public:
	MTargetCamera();
	~MTargetCamera();
};

