#pragma once
#include "MMesh.h"
class MMeshMgr : public MSingleton<MMeshMgr>
{
public:
	friend class MSingleton<MMeshMgr>;
public:
	map<M_STR, MMesh*> m_List;
	int				m_iIndexCount;
public:
	typedef std::map<M_STR, MMesh*>::iterator ITOR;
	MMesh*	operator [] (M_STR name);
public:
	//DWORD		Load(M_STR filename);
	MMesh*		Add(MMesh* data, M_STR name);
	bool		Release();
public:
	//bool Frame();
private:
	MMeshMgr();
public:
	~MMeshMgr();
};

#define I_MeshMgr MMeshMgr::GetInstance()