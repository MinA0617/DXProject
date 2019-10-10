#pragma once
#include "MTargetCamera.h"
#include "MInput.h"

class MRPGCamera : public MTargetCamera
{
private:
	POINT m_beforeMousePos;
	bool m_bIsClick;
public:
	bool Frame() override;
public:
	MRPGCamera();
	~MRPGCamera();
};

