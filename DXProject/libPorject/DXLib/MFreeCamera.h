#pragma once
#include "MCamera.h"

class MFreeCamera : public MCamera
{
	friend class MCameraMgr;
public:
	virtual void SetRotation(D3DXQUATERNION data);
	virtual void ModifyRotation(D3DXQUATERNION data);
public:
	virtual bool Init();
	virtual bool Frame();
public:
	MFreeCamera();
public:
	~MFreeCamera();
};

