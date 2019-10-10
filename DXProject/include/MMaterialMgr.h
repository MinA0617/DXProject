#pragma once
#include "MMaterial.h"

class MMaterialMgr : public MSingleton<MMaterialMgr>
{
	friend class MSingleton<MMaterialMgr>;
private:
	vector<MMaterial*>	m_List;
	//map<M_STR, MMaterial*> m_ObjList;
public:
	//typedef std::map<M_STR, M3DObject*>::iterator ITOR;
	MMaterial*			operator [] (DWORD index);
	DWORD				m_iCount;
public:
	DWORD				CreateNewMaterial();
public:
	virtual bool		Init();
	virtual bool		Frame();
	virtual bool		Release();
public:
	MMaterialMgr();
	~MMaterialMgr();
};

#define I_MaterialMgr MMaterialMgr::GetInstance()