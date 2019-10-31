#pragma once
#include "MStd.h"
#include "M3DObject.h"
#include "MBoundingBox.h"
#include "MCameraMgr.h"

struct MNode
{
	MBoundingBox			m_Box;
	bool					m_isLeaf;
	UINT					m_iDepth;
	vector<MNode*>			m_pChild;
	vector<M3DObject*>		m_pObj;
};

class MTree
{
public:
	MNode*		m_pRootNode;
	float		m_fMinDivideSize;
	bool		isOQ;
private:
	bool		BuildTree(MNode* pNode);
	bool		SubDivide(MNode* pNode);
	MNode*		CreateNode(float fMinX, float fMinY, float fMinZ, float fMaxX, float fMaxY, float fMaxZ, MNode* pParentNode = nullptr);
	bool		DeleteAll();
	bool		DeleteNode(MNode* pNode);
	bool		CheckRoot(M3DObject* pObj);
	bool		CheckNode(MNode pNode, M3DObject * pObj);
	bool		FindNode(MNode* pNode, M3DObject* pObj);
public:
	bool		AddObject(M3DObject* pObj);
	bool		Build(float fX, float fY, float fZ, float fMinSize = 5.0f);
	bool		SetCamera(MCamera* camera);
public:
	bool		Init();
	bool		Frame();
	bool		Render();
	bool		Release();
public:
	MTree();
	~MTree();
};

