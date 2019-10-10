#pragma once
#include "MObj.h"
class MObjMgr : public MSingleton<MObjMgr>
{
public:
	friend class MSingleton<MObjMgr>;
public:
	map<int, MObj*> m_List;
	int				m_iIndexCount;
public:
	typedef std::map<int, MObj*>::iterator ITOR;
	MObj*	operator [] (int iIndex);
	MObj*	operator [] (M_STR name);
public:
	DWORD		Load(M_STR filename);
	DWORD		Add(MObj* data, M_STR name);
	bool		Release();
private:
	MObjMgr();
public:
	~MObjMgr();
};

#define I_ObjMgr MObjMgr::GetInstance()