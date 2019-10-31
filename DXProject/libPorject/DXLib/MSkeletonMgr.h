#pragma once
#include "MSkeleton.h"

class MSkeletonMgr : public MSingleton<MSkeletonMgr>
{
	friend class MSingleton<MSkeletonMgr>;
private:
	typedef std::map<M_STR, MSkeleton* >::iterator ITOR;
	map<M_STR, MSkeleton*>	m_SktList;
	int						iCount;
public:
	MSkeleton* operator [] (M_STR name);
	MSkeleton*			Add(MSkeleton* data, M_STR name);
	bool				Delete(M_STR name);
public:
	virtual bool		Release();
private:
	MSkeletonMgr();
public:
	~MSkeletonMgr();
};

#define I_SkeletonMgr MSkeletonMgr::GetInstance()
