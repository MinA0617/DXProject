#pragma once
#include "MCore.h"

#if defined(DEBUG) || defined(_DEBUG)
#pragma comment (lib, "DXLib_D")
#else
#pragma comment (lib, "DXLib_R.lib")
#endif

#pragma comment (lib, "fmod_vc")

//struct ConstantFiled
//{
//	float g_FiledSize;
//	float g_TileSize;
//	float g_Heigth;
//	float temp;
//};
//
//struct filedvecter
//{
//	D3DXVECTOR2	Position;
//	int			DivisionCount;
//	int			NeighborType;
//};
//
//class Filed : public M3DModel
//{
//public:
//	ConstantFiled   constant;
//	MTexture		heigth;
//	ID3D11Buffer*	cb;
//	ID3D11Buffer*	vb;
//	virtual bool	Init();
//	virtual bool	Frame();
//	virtual bool	Render();
//	virtual bool	Release();
//public:
//	Filed() {};
//	~Filed() {}
//};

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
	Filed* f;

	MUnit* unit;
	MLight* light;
	//M3DModel* target;
	MFiled* filed;
	M3DObject* target;
	M3DModel* box1;
	M3DModel* box2;
	float y;
};

TCORE_RUN(asd,0,0,800,600)