#pragma once
#include "MStd.h"
#include "M3DObject.h"
#include "MBoundingBox.h"
#include "MCameraMgr.h"
#include "M3DHeightMap.h"

struct MTreeNode
{
	MBoundingBox			m_Box;
	bool					m_isLeaf;
	UINT					m_iDepth;
	MMapNode*				m_Tile;
	vector<MTreeNode*>		m_pChild;
	vector<M3DObject*>		m_pObj;
	bool					Render();
	bool					MapRender();
	MTreeNode() { m_Tile = nullptr; };
};

class MTree
{
public:
	vector<MTreeNode*> m_RenderNodeList;
	MTreeNode*	m_pRootNode;
	float		m_fMinDivideSize;
	DWORD		m_dwMaxDepth;
	int			m_DepthLevelOfMapNode; // 맵의 노드가 포함된 댑스 레벨
	bool		isOQ;
private:
	bool		BuildTree(MTreeNode* pNode);
	bool		SubDivide(MTreeNode* pNode);
	MTreeNode*	CreateNode(float fMinX, float fMinZ, float fMaxX, float fMaxZ, MTreeNode* pParentNode = nullptr);
	bool		DeleteAll();
	bool		DeleteNode(MTreeNode* pNode);
	bool		CheckRoot(M3DObject* pObj);
	//bool		CheckNode(MTreeNode pNode, M3DObject * pObj);
	//bool		FindNode(MTreeNode* pNode, M3DObject* pObj);
public:
	bool		AddObject(M3DObject* pObj);
	bool		Build(float fX, float fZ, float fMinSize = 5.0f);
	bool		BuildHeightMap(M3DHeightMap* target);
	bool		SetMaxY(MTreeNode* pNode, M3DHeightMap* target);
	bool		FindMapNode(MTreeNode* node, M3DHeightMap* target);
	//bool		SetCamera(MCamera* camera);
	int			CheckNode(MTreeNode* node);
public:
	//bool		Init();
	bool		Frame();
	bool		Render();
	bool		Release();
public:
	MTree();
	~MTree();
};

