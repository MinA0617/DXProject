#pragma once
#include "M3DBone.h"
#include "MKeyAnimation.h"

class MSkeleton : public M3DObject
{
public:
	typedef map<M_STR, M3DBone*>::iterator ITOR;
	map<M_STR, M3DBone*>	m_BoneList;
	ID3D11Buffer*			m_pBoneBuffer;
	vector<CONSTANT_BONE>	m_ConstantBone;
	float					m_fAniTime;
	float					m_fAniSpeed;
public:
	bool					CreateBoneBuffer();
	M3DBone*				NewBone(M_STR name);
	bool					BindAni(MKeyAnimation* data);
	int						NumofBone(M_STR name);
public:
	bool					Set();
	virtual bool			Init();
	virtual bool			Frame();
	virtual bool			Render();
	virtual bool			Release();
	MSkeleton();
	~MSkeleton();
};

