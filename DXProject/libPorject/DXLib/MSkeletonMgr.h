#pragma once
#include "MSkeleton.h"

class MSkeletonMgr : public MSingleton<MSkeletonMgr>
{
	friend class MSingleton<MSkeletonMgr>;
private:
	typedef std::map<int, MSkeleton* >::iterator ITOR;
	map<int, MSkeleton*>	m_ObjList;
	int						iCount;
public:
	MSkeleton* operator [] (int index);
	int					Add(MSkeleton* data);
	bool				Delete(int index);
public:
	//virtual bool		Init();
	virtual bool		Frame();
	virtual bool		Render();
	virtual bool		Release();
private:
	MSkeletonMgr();
public:
	~MSkeletonMgr();
};

#define I_SkeletonMgr MSkeletonMgr::GetInstance()
