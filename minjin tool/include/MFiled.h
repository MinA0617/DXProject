#pragma once
#include "M3DObject.h"
#include "M3DHeightMap.h"
class MFiled : public M3DObject
{
public:
	M3DHeightMap* ground;
public:
	bool	Set(M_STR name, M3DHeightMap* map);
	bool	Init();
	bool	Frame();
	bool	Render();
	bool	Release();
public:
	MFiled();
	~MFiled();
};

