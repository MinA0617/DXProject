#pragma once
#include "MKeyAnimation.h"

class MKeyAnimationMgr : public MSingleton<MKeyAnimationMgr>
{
	friend class MSingleton<MKeyAnimationMgr>;
public:
	typedef map<M_STR, MKeyAnimation*>::iterator ITOR;
	map<M_STR, MKeyAnimation*>	m_KeyAniList;
	MKeyAnimation*				NewKeyAnimation(M_STR name);
	//bool						Load(M_STR filename);
	bool						Release();
	MKeyAnimation* operator	[](M_STR name);
private:
	MKeyAnimationMgr();
public:
	~MKeyAnimationMgr();
};

#define I_KeyAnimationMgr MKeyAnimationMgr::GetInstance()