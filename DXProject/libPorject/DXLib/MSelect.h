#pragma once
#include "MStd.h"
#include "MCollision.h"
#include "M3DObjectMgr.h"

class MSelect
{
private:
	static D3DXVECTOR2		GetScreenPoint();
	static MRAY				GetScreenRay();
	static bool				CheckTri(MTreeNode* pNode, MRAY* ray, D3DXVECTOR3* minintersection = nullptr, DWORD* index = nullptr);
	static void				CheckNode(MTreeNode* pNode, MRAY* ray, vector<MTreeNode*>& list);
	static void				CheckUPNode(MTreeNode* pNode, MRAY* ray, vector<MTreeNode*>& list); // y축값 늘려서 계산
	static bool				CheckNodeList(vector<MTreeNode*>& list);
	static bool				CheckUPNodeList(vector<MTreeNode*>& list); // y축값 늘려서 계산 and 타일레벨이 아닌 상위노드도 체크
public:
	static M3DInstance*		PickObject(DWORD* ModelID = nullptr, DWORD* InstanceID = nullptr, D3DXVECTOR3* intersection = nullptr);
	static MMapNode*		PickMapNode();
	static bool				PickGroundPosition(D3DXVECTOR3* result = nullptr, DWORD* index = nullptr);
public:
	MSelect();
	~MSelect();
};
