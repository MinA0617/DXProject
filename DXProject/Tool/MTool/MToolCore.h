#pragma once
#include "MCore.h"

#if defined(DEBUG) || defined(_DEBUG)
#pragma comment (lib, "DXLib_D")
#else
#pragma comment (lib, "DXLib_R.lib")
#endif

#pragma comment (lib, "fmod_vc")
#include "MPushPull.h"
#include "MCanvas.h"
#include "MTileing.h"
#include "MCreateInstance.h"

enum ModifyerState { NOT_SET = 0, TILEING, BRUSH, PUSH, CREATEOBJ, DELETEOBJ };

class MToolCore : public MCore
{
public:
	M3DInstanceModel* inmo;
	// Modifyer
	ModifyerState	m_State;
	MTileing	tileing;
	MPushPull	pushpull;
	MCanvas		canvas;
	MCreateInstance instance;
public:
	MDXWirte*	data;
	// Object
	MLight* light;
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

