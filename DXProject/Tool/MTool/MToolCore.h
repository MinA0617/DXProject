#pragma once
#include "MCore.h"
#include "MParser.h"
#include "MLight.h"
#include "MTree.h"
#include "MCollision.h"
#include "MBBParser.h"

#if defined(DEBUG) || defined(_DEBUG)
#pragma comment (lib, "DXLib_D")
#else
#pragma comment (lib, "DXLib_R.lib")
#endif

#pragma comment (lib, "fmod_vc")


class MToolCore : public MCore
{
public:
	MParser ps;
	MUnit* unit;
	MLight* light;
	//M3DModel* target;
	MFiled* filed;
	M3DObject* target;
	float y;
public:
	bool	Init();
	bool	Frame();
	bool	Render();
	bool	Release();
public:
	MToolCore();
	~MToolCore();
};

