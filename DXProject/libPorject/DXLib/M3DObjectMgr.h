#pragma once
#include "M3DObject.h"
#include "MCameraMgr.h"
#include "MLightMgr.h"
#include "MSkeletonMgr.h"

class M3DObjectMgr : public MSingleton<M3DObjectMgr>
{
	friend class MSingleton<M3DObjectMgr>;
private:
	typedef std::map<int, M3DObject*>::iterator ITOR;
	map<int, M3DObject*> m_ObjList;
	int					iCount;
public:
	M3DObject* operator [] (M_STR name);
	M3DObject* operator [] (int index);
	bool				Add(M3DObject* data);
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
