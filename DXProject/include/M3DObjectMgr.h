#pragma once
#include "M3DObject.h"
#include "MCameraMgr.h"
#include "MLightMgr.h"
#include "MSkeletonMgr.h"
#include "MUint.h"
#include "M3DBasic.h"
#include "MFiled.h"

class M3DObjectMgr : public MSingleton<M3DObjectMgr>
{
	friend class MSingleton<M3DObjectMgr>;
private:
	typedef std::map<M_STR, M3DObject*>::iterator ITOR;
	typedef std::map<M_STR, MUnit*>::iterator ITORU;
	typedef std::map<M_STR, MFiled*>::iterator ITORF;
	typedef std::map<int, M3DModel*>::iterator ITORO;
	map<M_STR, M3DObject*>	m_List;
public:
	map<M_STR, MUnit*>	m_InWorldUnitList;
	map<M_STR, MFiled*>	m_InWorldFiledList;
	map<int, M3DModel*>	m_InWorldObjectList;
	int					m_iObjIndex;
public:
	M3DObject*			find(M_STR name);
	MUnit*				findUnit(M_STR name);
	MFiled*				findFiled(M_STR name);
	M3DModel*			findObject(int index);
public:
	//M3DObject*			m_pWorld;
	M3DObject* operator [] (M_STR name);
	bool				Add(M3DObject* data);
	bool				Delete(M_STR name);
public:
	MUnit*				CreateUnit(M_STR name, M_STR sktname, M_STR* namelist, int namecount);
	MFiled*				CreateFiled(M3DHeightMap* map);
	int					AddInWorld(M_STR* namelist, int namecount = 1);
	bool				CreateBasicBuffer();
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
