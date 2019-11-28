#pragma once
#include "M3DObject.h"
#include "MCameraMgr.h"
#include "MLightMgr.h"
#include "MSkeletonMgr.h"
#include "MUint.h"
#include "M3DBasic.h"
#include "MFiled.h"
#include "MTree.h"

class M3DObjectMgr : public MSingleton<M3DObjectMgr>
{
	friend class MSingleton<M3DObjectMgr>;
private:
	typedef std::map<M_STR, M3DObject*>::iterator ITOR;
	typedef std::map<M_STR, MUnit*>::iterator ITORU;
	typedef std::map<M_STR, MFiled*>::iterator ITORF;
	typedef std::map<int, M3DModel*>::iterator ITORO;
	map<M_STR, M3DObject*>	m_List;
	map<M_STR, MFiled*>		m_FiledList;
public:
	MTree*					m_pTree;
	MFiled*					m_InWorldFiled;
	map<M_STR, MUnit*>		m_InWorldUnitList;
public:
	M3DObject*			find(M_STR name);
	MUnit*				findUnit(M_STR name);
	MFiled*				findFiled(M_STR name);
public:
	//M3DObject*			m_pWorld;
	M3DObject* operator [] (M_STR name);
	bool				Add(M3DObject* data);
	bool				Delete(M_STR name);
private:
	bool				CreateTree();
	bool				SetFiled(M_STR name);
public:
	MUnit*				CreateUnit(M_STR name, M_STR sktname, M_STR* namelist, int namecount);
	MFiled*				CreateFiled(M3DHeightMap* map, bool isChange = true);
	MFiled*				CreateFiled(M_STR name, int count, float leafsize = 10, int tilesize = 64, float startlod = 1000, bool isChange = true);

	//int				AddInWorld(M_STR* namelist, int namecount = 1);
	//bool				DeleteInWorld(int index);
	bool				CreateBasicBuffer();
	//--- test ---
private:
	map<M_STR, int>				InstanceModelNameTable;
	map<int, M3DInstanceModel*> InstanceModelList;
public:
	int							m_iInstanceListCount;
	int							AddInstanceModel(M3DInstanceModel* model);
	int							AddInstanceObj(int id, int count = 1);
	int							AddInstanceObj(int id, D3DXVECTOR3* pos, D3DXQUATERNION* rot, D3DXVECTOR3* scl);
	int							GetInstanceModelID(M_STR name);
	M3DInstanceModel*			GetInstanceModel(M_STR name);
	M3DInstanceModel*			GetInstanceModel(int id);
	M3DInstance*				GetInstanceObj(int id, int lowid);
	
	bool						InstanceRender();
	// ------------
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
