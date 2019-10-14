#pragma once
#include "MMesh.h"
class MMeshMgr : public MSingleton<MMeshMgr>
{
public:
	friend class MSingleton<MMeshMgr>;
public:
	map<int, MMesh*> m_List;
	int				m_iIndexCount;
public:
	typedef std::map<int, MMesh*>::iterator ITOR;
	MMesh*	operator [] (int iIndex);
public:
	//DWORD		Load(M_STR filename);
	DWORD		Add(MMesh* data);
	bool		Release();
private:
	MMeshMgr();
public:
	~MMeshMgr();
};

#define I_MeshMgr MMeshMgr::GetInstance()