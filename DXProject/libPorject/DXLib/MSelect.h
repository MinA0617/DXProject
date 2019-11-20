#pragma once
#include "MStd.h"
#include "MCollision.h"
#include "M3DObjectMgr.h"

class MSelect
{
public:
	static D3DXVECTOR2		GetScreenPoint();
	static MRAY				GetScreenRay();
	static bool				CheckTri(MTreeNode* pNode, MRAY* ray, D3DXVECTOR3* minintersection = nullptr, DWORD* index = nullptr);
	static MTreeNode*		CheckNode(MTreeNode* pNode, MRAY* ray);
public:
	static M3DObject*		PickObject();
	static MMapNode*		PickTile();
	static bool				PickGroundPosition(D3DXVECTOR3* result);
public:
	MSelect();
	~MSelect();
};
