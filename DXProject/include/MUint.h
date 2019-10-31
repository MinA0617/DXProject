#pragma once
#include "MSkinModel.h"
#include "MSkeletonMgr.h"

#define MAX_OBJ 16
enum ITEM_TYPE {EYES, HEAD, HAIR, UPPERBODY, LOWERBODY, HANDS, FEET};


class MUnit : public M3DObject
{
public:
	M_STR sktname;
	MSkeleton* skt;
	MSkinModel* obj[MAX_OBJ];
	int			m_iCount;
public:
	bool Create(M_STR name, M_STR sktname, M_STR* namelist, int namecount);
	bool Change(ITEM_TYPE type, M_STR name);
	void UpdateBox() override;
	bool Init() override;
	bool Frame() override;
	bool Render() override;
	bool Release() override;
public:
	MUnit();
	~MUnit();
};

