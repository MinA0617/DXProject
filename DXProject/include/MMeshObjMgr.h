#pragma once
#include "MMeshObj.h"
class MMeshObjMgr : public MSingleton<MMeshObjMgr>
{
public:
	friend class MSingleton<MMeshObjMgr>;
public:
	map<int, MMeshObj*> m_List;
	int				m_iIndexCount;
public:
	typedef std::map<int, MMeshObj*>::iterator ITOR;
	MMeshObj*	operator [] (int iIndex);
	MMeshObj*	operator [] (M_STR name);
public:
	DWORD		Load(M_STR filename);
	DWORD		Add(MMeshObj* data, M_STR name);
	bool		Release();
private:
	MMeshObjMgr();
public:
	~MMeshObjMgr();
};

#define I_MeshObjMgr MMeshObjMgr::GetInstance()