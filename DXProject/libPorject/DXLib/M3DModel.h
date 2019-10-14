#pragma once
#include "M3DObject.h"
#include "MMesh.h"

class M3DModel : public M3DObject
{
	friend class MParser;
public:
	//bool			CreateObj(MMesh* data);
public:
	//bool			Load(M_STR filename);
	bool			Load_MAP(M_STR filename, MAPTYPE type);
public:
	virtual bool	Init();
	virtual bool	Frame();
	virtual bool	Render();
	virtual bool	Release();
public:
	M3DModel();
	~M3DModel();
};

