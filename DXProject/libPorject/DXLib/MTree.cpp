#include "MTree.h"
#include "M3DObjectMgr.h"


bool MTree::AddObject(M3DObject * pObj)
{
	if (CheckRoot(pObj))
	{
	
	}
	return false;
}

bool MTree::Build(float fX, float fZ, float fMinSize)
{
	m_pRootNode = CreateNode(-fX / 2, -fZ / 2, fX / 2, fZ / 2);
	m_fMinDivideSize = fMinSize;
	return BuildTree(m_pRootNode);
}

bool MTree::BuildHeightMap(M3DHeightMap* target)
{
	m_DepthLevelOfMapNode = target->m_iMaxLevel - target->m_iLodMinLevel;
	float size = target->m_fLeafSize;
	float xsize = size * target->m_iCount - size;
	float zsize = size * target->m_iCount - size;
	Build(xsize, zsize, size);
	SetMaxY(m_pRootNode, target);
	//SetNode(target);
	return true;
}


bool MTree::SetMaxY(MTreeNode * pNode, M3DHeightMap * target)
{
	if (pNode->m_isLeaf == true)
	{
		pNode->m_Box.vMax.y = target->FindLeafYMax(pNode->m_Box.vMin.x, -pNode->m_Box.vMax.z);
	}
	else
	{
		float topy[4];
		for (int i = 0; i < pNode->m_pChild.size(); i++)
		{
			SetMaxY(pNode->m_pChild[i], target);
			topy[i] = pNode->m_pChild[i]->m_Box.vMax.y;
		}
		pNode->m_Box.vMax.y = max(max(topy[0], topy[1]), max(topy[2], topy[3]));
	}
	if (pNode->m_iDepth == m_DepthLevelOfMapNode)
	{
		FindMapNode(pNode, target);
	}
	return true;
}

bool MTree::FindMapNode(MTreeNode* node, M3DHeightMap * target)
{
	for (auto tile : target->m_List)
	{
		if (tile->m_Box->vMax.x == node->m_Box.vMax.x&&tile->m_Box->vMin.x == node->m_Box.vMin.x)
		{
			if (tile->m_Box->vMax.z == node->m_Box.vMax.z&&tile->m_Box->vMin.z == node->m_Box.vMin.z)
			{
				node->m_Tile = tile;
			}
		}
	}

	return true;
}


bool  MTree::BuildTree(MTreeNode* pNode)
{
	if (SubDivide(pNode))
	{
		for (int i = 0; i < pNode->m_pChild.size(); i++)
		{
			BuildTree(pNode->m_pChild[i]);
		}
	}
	return true;
}

MTreeNode*	MTree::CreateNode(float fMinX, float fMinZ, float fMaxX, float fMaxZ, MTreeNode* pParentNode)
{
	MTreeNode* pNode = new MTreeNode;
	pNode->m_pChild.reserve(0);

	pNode->m_Box.vMin.x = fMinX;
	pNode->m_Box.vMin.y = 0;
	pNode->m_Box.vMin.z = fMinZ;

	pNode->m_Box.vMax.x = fMaxX;
	pNode->m_Box.vMax.y = 0;
	pNode->m_Box.vMax.z = fMaxZ;

	pNode->m_Box.vOldCenter.x = (fMinX + fMaxX) / 2;
	pNode->m_Box.vOldCenter.y = 0;
	pNode->m_Box.vOldCenter.z = (fMinZ + fMaxZ) / 2;

	pNode->m_Box.vAxis[0] = D3DXVECTOR3(1, 0, 0);
	pNode->m_Box.vAxis[1] = D3DXVECTOR3(0, 1, 0);
	pNode->m_Box.vAxis[2] = D3DXVECTOR3(0, 0, 1);

	pNode->m_Box.fOldExtent[0] = (fMaxX - fMinX) / 2;
	pNode->m_Box.fOldExtent[1] = (fMaxX - fMinX) / 2;
	pNode->m_Box.fOldExtent[2] = (fMaxX - fMinX) / 2;

	pNode->m_Box.vCenter.x = (fMinX + fMaxX) / 2;
	pNode->m_Box.vCenter.y = 0;
	pNode->m_Box.vCenter.z = (fMinZ + fMaxZ) / 2;

	pNode->m_Box.fExtent[0] = pNode->m_Box.fOldExtent[0];
	pNode->m_Box.fExtent[1] = pNode->m_Box.fOldExtent[1];
	pNode->m_Box.fExtent[2] = pNode->m_Box.fOldExtent[2];

	if (pParentNode != nullptr)
	{
		pNode->m_iDepth = pParentNode->m_iDepth + 1;
	}
	else
	{
		pNode->m_iDepth = 0;
	}
	return pNode;
}

bool MTree::DeleteAll()
{
	if (m_pRootNode != nullptr)
	{
		DeleteNode(m_pRootNode);
		return true;
	}
	return false;
}

bool MTree::DeleteNode(MTreeNode * pNode)
{
	int Size = pNode->m_pChild.size();
	for (int i = 0; i < Size; i++)
	{
		DeleteNode(pNode->m_pChild[i]);
	}
	delete pNode;
	return true;
}

bool MTree::CheckRoot(M3DObject * pObj)
{
	//if (m_pRootNode->m_Box.vMin.x <= pObj->m_BoxList->vMin.x && m_pRootNode->m_Box.vMax.x >= pObj->m_BoxList->vMax.x)
	//{
	//	if (m_pRootNode->m_Box.vMin.y <= pObj->m_BoxList->vMin.y && m_pRootNode->m_Box.vMax.y >= pObj->m_BoxList->vMax.y)
	//	{
	//		if (m_pRootNode->m_Box.vMin.z <= pObj->m_BoxList->vMin.z && m_pRootNode->m_Box.vMax.z >= pObj->m_BoxList->vMax.z)
	//		{
	//			return true;
	//		}
	//	}
	//}
	return false;
}

//bool MTree::CheckNode(MTreeNode pNode, M3DObject * pObj)
//{
//	//D3DVECTOR point[8];
//	//if (m_pRootNode->m_Box.vMin.x <= pObj->m_BoxList->vMin.x && m_pRootNode->m_Box.vMax.x >= pObj->m_BoxList->vMax.x)
//	//{
//	//	if (m_pRootNode->m_Box.vMin.y <= pObj->m_BoxList->vMin.y && m_pRootNode->m_Box.vMax.y >= pObj->m_BoxList->vMax.y)
//	//	{
//	//		if (m_pRootNode->m_Box.vMin.z <= pObj->m_BoxList->vMin.z && m_pRootNode->m_Box.vMax.z >= pObj->m_BoxList->vMax.z)
//	//		{
//	//			return true;
//	//		}
//	//	}
//	//}
//	return false;
//}

bool MTree::SubDivide(MTreeNode* pNode)
{
	float fx = (pNode->m_Box.vMax.x - pNode->m_Box.vMin.x);
	float fz = (pNode->m_Box.vMax.z - pNode->m_Box.vMin.z);
	if (m_fMinDivideSize >= fx && m_fMinDivideSize >= fz)
	{
		pNode->m_isLeaf = true;
		m_dwMaxDepth = pNode->m_iDepth;
		return false;
	}
	pNode->m_isLeaf = false;
	pNode->m_pChild.reserve(0);
	MBoundingBox pb;
	float x[3] = { pNode->m_Box.vMin.x, pNode->m_Box.vMin.x + (fx / 2), pNode->m_Box.vMax.x };
	float z[3] = { pNode->m_Box.vMin.z, pNode->m_Box.vMin.z + (fz / 2), pNode->m_Box.vMax.z };
	MTreeNode* pChildNode[4];
	pChildNode[0] = CreateNode(x[0], z[1], x[1], z[2], pNode);
	pChildNode[1] = CreateNode(x[1], z[1], x[2], z[2], pNode);
	pChildNode[2] = CreateNode(x[1], z[0], x[2], z[1], pNode);
	pChildNode[3] = CreateNode(x[0], z[0], x[1], z[1], pNode);

	for (int i = 0; i < 4; i++)
	{
		pNode->m_pChild.push_back(pChildNode[i]);
	}
	return true;
}

int MTree::CheckNode(MTreeNode * node)
{
	int result = I_CameraMgr.frustum.CheckNode(node);
	switch (result)
	{
	case -1:
	{
		return false;
	}break;
	case 0:
	{
		if (node->m_iDepth == m_DepthLevelOfMapNode)
		{
			m_RenderNodeList.push_back(node);
			return true;
		}
		for (int i = 0; i < node->m_pChild.size(); i++)
		{
			CheckNode(node->m_pChild[i]);
		}
	}break;
	case 1:
	{
		m_RenderNodeList.push_back(node);
		return true;
	}break;
	default:
		break;
	}
}


bool MTree::Frame()
{
	m_RenderNodeList.clear();
	CheckNode(m_pRootNode);
	return true;
}

bool MTree::Render()
{
	I_3DObjectMgr.m_InWorldFiled->Render();
	for (auto node : m_RenderNodeList)
	{
		node->Render();
	}
	return true;
}

bool MTree::Release()
{
	DeleteAll();
	return true;
}

MTree::MTree()
{
	m_pRootNode = nullptr;
	m_dwMaxDepth = 0;
}


MTree::~MTree()
{
}

bool MTreeNode::Render()
{
	if (m_Tile) m_Tile->Render();
	for (auto obj : m_pObj)
	{
		obj->Render();
	}
	for (auto Child : m_pChild)
	{
		Child->Render();
	}
	return true;
}
