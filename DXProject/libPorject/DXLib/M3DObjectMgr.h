#pragma once
#include "M3DObject.h"
#include "MCameraMgr.h"
#include "MLightMgr.h"
#include "MSkeletonMgr.h"

class M3DObjectMgr : public MSingleton<M3DObjectMgr>
{
	friend class MSingleton<M3DObjectMgr>;
private:
	map<M_STR, M3DObject*> m_ObjList;
public:
	typedef std::map<M_STR, M3DObject*>::iterator ITOR;
	M3DObject* operator [] (M_STR name);
	bool				Add(M_STR name, M3DObject* data);
	bool				Delete(M_STR name);
public:
	virtual bool		Init();
	virtual bool		Frame();
	virtual bool		Render();
	virtual bool		Release();
private:
	M3DObjectMgr();
public:
	~M3DObjectMgr();
};

#define I_3DObjectMgr M3DObjectMgr::GetInstance()
