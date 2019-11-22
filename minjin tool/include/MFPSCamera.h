#pragma once
#include "MFreeCamera.h"
#include "MInput.h"

class MFPSCamera : public MFreeCamera
{
	friend class MCameraMgr;
public:
	BOOL isCenterMouse;
public:
	bool Frame() override;
public:
	MFPSCamera();
public:
	~MFPSCamera();
};

