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
	MMesh*	operator [] (M_STR name);
public:
	//DWORD		Load(M_STR filename);
	DWORD		Add(MMesh* data, M_STR name);
	bool		Release();
private:
	MMeshMgr();
public:
	~MMeshMgr();
};

#define I_MeshMgr MMeshMgr::GetInstance()