#pragma once
#include "M3DInstance.h"
#include "MSkeletonMgr.h"

#define MAX_OBJ 16
enum ITEM_TYPE {EYES, HEAD, HAIR, UPPERBODY, LOWERBODY, HANDS, FEET};


class MUnit : public M3DNObject
{
public:
	M_STR							m_name;
	MSkeleton*						m_pSkt;
	map<ITEM_TYPE, M3DNObject*>		m_ObjList;
public:
	bool Create(M_STR name, M_STR sktname, M_STR* namelist, int namecount);
	bool Change(ITEM_TYPE type, M_STR name);
	bool Frame();
	bool Render();
	bool Release();
public:
	MUnit();
	~MUnit();
};

