#pragma once
#include "MObject.h"

class M3DObj : public MObject
{
public:
	M3Point					m_WorldPos;
	M3Point					m_WorldRotation;
	M3Point					m_WorldScale;
public:
	virtual bool Init();
	virtual bool Frame();
	virtual bool Render();
	virtual bool Release();
public:
	M3DObj();
	~M3DObj();
};

