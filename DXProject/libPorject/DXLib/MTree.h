#pragma once
#include "MStd.h"
#include "M3DObject.h"
#include "MBoundingBox.h"
#include "MCameraMgr.h"
#include "M3DHeightMap.h"
#include "M3DInstanceModel.h"

struct MTreeNode
{
	MBoundingBox			m_Box;
	bool					m_isLeaf;
	UINT					m_iDepth;
	MMapNode*				m_Tile;
	vector<MTreeNode*>		m_pChild;
	vector<M3DObject*>		m_pObjList;
	set<M3DInstance*>		m_pInstanceList;
	bool					Render();
	bool					MapRender();
	MTreeNode() { m_Tile = nullptr; };
};

class MTree
{
public:
	vector<MTreeNode*>		m_RenderNodeList;
	vector<M3DInstance*>	m_RenderObjList;
	MTreeNode*				m_pRootNode;
	float					m_fMinDivideSize;
	DWORD					m_dwMaxDepth;
	int						m_DepthLevelOfMapNode; // ���� ��尡 ���Ե� �� ����
	bool					isOQ;
private:
	bool		BuildTree(MTreeNode* pNode);
	bool		SubDivide(MTreeNode* pNode);
	MTreeNode*	CreateNode(float fMinX, float fMinZ, float fMaxX, float fMaxZ, MTreeNode* pParentNode = nullptr);
	bool		DeleteAll();
	bool		DeleteNode(MTreeNode* pNode);
public:
	bool		Build(float fX, float fZ, float fMinSize = 10.0f, int renderlevel = 4);
	bool		BuildHeightMap(M3DHeightMap* target);
	bool		SetMaxY(MTreeNode* pNode, M3DHeightMap* target);
	bool		FindMapNode(MTreeNode* node, M3DHeightMap* target);
	// --- test ----
	map<M3DInstance*, MTreeNode*> m_InstanceObjTable;
	bool		CheckAABB(MBoundingBox* box);
	int			CheckNode(MTreeNode* node);
	int			CheckInstanceObj(MTreeNode* pNode, M3DInstance* data);
	bool		CheckInstanceObject(M3DInstance* data);
	bool		DeleteInstancObject(M3DInstance* data);
public:
	//bool		Init();
	bool		Frame();
	bool		Render();
	bool		Release();
public:
	MTree();
	~MTree();
};

