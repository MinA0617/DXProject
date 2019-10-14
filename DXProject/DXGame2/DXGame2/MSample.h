#pragma once
#include "MCore.h"
#include "MParser.h"
#include "MLight.h"

#if defined(DEBUG) || defined(_DEBUG)
#pragma comment (lib, "DXLib_D")
#else
#pragma comment (lib, "DXLib_R.lib")
#endif

#pragma comment (lib, "fmod_vc")


class MSample : public MCore
{
public:
	bool	Init();
	bool	Frame();
	bool	Render();
	bool	Release();
public:
	MSample();
	~MSample();
public:
	MParser ps;
	MSkeleton* target;
	MLight* light;
	float y;
};

TCORE_RUN(asd,0,0,800,600)