#pragma once
#include "MCore.h"
#include "MHM.h"
#include "M3DInstanceModel.h"

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
	M3DInstanceModel* inmo;
	MDXWirte*	data;
	////MHM hm;
	//MUnit* unit;
	//MLight* light;
	////M3DModel* target;
	//MFiled* filed;
	//M3DObject* target;
	//M3DModel* box1;
	//M3DModel* box2;
	float y;
};

TCORE_RUN(asd,0,0,800,600)