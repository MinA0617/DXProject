#pragma once
#include "MTextureMgr.h"
#include "MMaterialMgr.h"

class MObject
{
public:
	DWORD				MaterialID;
protected:
	ID3D11Buffer*		m_pConstantBuffer;
public:
	MMaterial*		GetMatarial();
	virtual bool	Init() = 0;
	virtual bool	Frame() = 0;
	virtual bool	Render() = 0;
	virtual bool	Release() = 0;
public:
	MObject();
	~MObject();
};

