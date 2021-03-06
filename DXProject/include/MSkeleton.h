#pragma once
#include "M3DBone.h"
#include "MKeyAnimationMgr.h"

class MSkeleton : public M3DObject
{
public:
	typedef map<int, M3DBone*>::iterator ITOR;
	map<int, M3DBone*>		m_BoneList;
	//map<M_STR, D3DXMATRIX>	m_ZeroMat;
	int						iCount;
	//vector<M3DBone*>		m_BoneList;
	ID3D11Buffer*			m_pBoneBuffer;
	ID3D11Buffer*			m_pZeroBuffer;
	vector<D3DXMATRIX>		m_Coculration;
	vector<CONSTANT_BONE>	m_ConstantBone;
	vector<CONSTANT_BONE>	m_ConstantZero;
	bool					m_bIsSlerp;
	float					m_fDelay;
	float					m_fAniSpeed;
	float					m_fAniTime;
	float					m_fMaxTime;
	M_STR					m_CurAni;
	M_STR					m_NextAni;
public:
	bool					CreateBoneBuffer();
	bool					BindAni(M_STR name, bool isCancel = true, float delay = 0);
	M3DBone*				NewBone(M_STR name);
	M3DBone*				Find(M_STR name);
	void					SetZeroMat();
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

