#include "MTree.h"
#include "M3DObjectMgr.h"

bool MTree::Build(float fX, float fZ, float fMinSize, int renderlevel)
{
	m_DepthLevelOfMapNode = renderlevel;
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
	Build(xsize, zsize, size, m_DepthLevelOfMapNode);
	SetMaxY(m_pRootNode, target);
	//SetNode(target);
	return true;
}


bool MTree::SetMaxY(MTreeNode * pNode, M3DHeightMap * target)
{
	if (pNode->m_isLeaf == true)
	{
		pNode->m_Box.UpdataMaxY(target->FindLeafYMax(pNode->m_Box.vMin.x, pNode->m_Box.vMin.z));
	}
	else
	{
		float topy[4];
		for (int i = 0; i < pNode->m_pChild.size(); i++)
		{
			SetMaxY(pNode->m_pChild[i], target);
			topy[i] = pNode->m_pChild[i]->m_Box.vMax.y;
		}
		pNode->m_Box.UpdataMaxY(max(max(topy[0], topy[1]), max(topy[2], topy[3])));
	}
	if (pNode->m_iDepth == m_DepthLevelOfMapNode)
	{
		if (pNode->m_Tile == nullptr)
		{
			FindMapNode(pNode, target);
		}
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
#if defined(DEBUG) || defined(_DEBUG)
	pNode->m_Box.Init();
#endif
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
	pNode->m_Box.fOldExtent[1] = 0;
	pNode->m_Box.fOldExtent[2] = (fMaxX - fMinX) / 2;

	pNode->m_Box.vCenter.x = (fMinX + fMaxX) / 2;
	pNode->m_Box.vCenter.y = 0;
	pNode->m_Box.vCenter.z = (fMinZ + fMaxZ) / 2;

	pNode->m_Box.fExtent[0] = pNode->m_Box.fOldExtent[0];
	pNode->m_Box.fExtent[1] = pNode->m_Box.fOldExtent[1];
	pNode->m_Box.fExtent[2] = pNode->m_Box.fOldExtent[2];

	pNode->m_Box.SelfUpdate();

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
	//pChildNode[0] = CreateNode(x[0], z[0], x[1], z[1], pNode);
	//pChildNode[1] = CreateNode(x[1], z[0], x[2], z[1], pNode);
	//pChildNode[2] = CreateNode(x[1], z[1], x[2], z[2], pNode);
	//pChildNode[3] = CreateNode(x[0], z[1], x[1], z[2], pNode);

	for (int i = 0; i < 4; i++)
	{
		pNode->m_pChild.push_back(pChildNode[i]);
	}
	return true;
}

bool MTree::CheckAABB(MBoundingBox * box)
{
	return abs(box->vAxis[0].x - 1.00000) < 0.0001;
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
	case 0: // 걸친경우
	{
		if (node->m_iDepth == m_DepthLevelOfMapNode)
		{
			m_RenderNodeList.push_back(node);
			return true;
		}
		else
		{
			for (auto temp : node->m_pNObjList)
			{
				if (I_CameraMgr.frustum.CheckOBB(&temp->m_Box))
				{
					m_RenderObjList.push_back(temp);
				}
			}
			for (int i = 0; i < node->m_pChild.size(); i++)
			{
				CheckNode(node->m_pChild[i]);
			}
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

int MTree::CheckInstanceObj(MTreeNode* pNode, M3DNObject * data)
{
	if (CheckAABB(&data->m_Box))
	{
		if (pNode->m_Box.vMin.x <= data->m_Box.vMin.x && pNode->m_Box.vMax.x >= data->m_Box.vMax.x)
		{
			if (pNode->m_Box.vMin.z <= data->m_Box.vMin.z && pNode->m_Box.vMax.z >= data->m_Box.vMax.z)
			{
				if (pNode->m_iDepth == m_DepthLevelOfMapNode)
				{
					map<M3DNObject*, MTreeNode*>::iterator temp = m_InstanceObjTable.find(data);
					if (temp != m_InstanceObjTable.end())
					{
						(*temp).second->m_pNObjList.erase(data);
						m_InstanceObjTable.erase(data);
					}
					m_InstanceObjTable.insert(make_pair(data, pNode));
					pNode->m_pNObjList.insert(data);
					return true;
				}
				else
				{
					for (auto temp : pNode->m_pChild)
					{
						if (CheckInstanceObj(temp, data))
						{
							return true;
						}
					}
					map<M3DNObject*, MTreeNode*>::iterator temp = m_InstanceObjTable.find(data);
					if (temp != m_InstanceObjTable.end())
					{
						(*temp).second->m_pNObjList.erase(data);
						m_InstanceObjTable.erase(data);
					}
					m_InstanceObjTable.insert(make_pair(data, pNode));
					pNode->m_pNObjList.insert(data);
					return true;
				}
			}
		}
		return false;
	}
	else
	{
		for (int i = 0; i < 8; i++)
		{
			if ((pNode->m_Box.vMin.x <= data->m_Box.vPoint[i].x && pNode->m_Box.vMax.x >= data->m_Box.vPoint[i].x))
			{
				if (!(pNode->m_Box.vMin.z <= data->m_Box.vPoint[i].z && pNode->m_Box.vMax.z >= data->m_Box.vPoint[i].z))
				{
					return false;
				}
			}
			else
			{
				return false;
			}
		}
		if (pNode->m_iDepth == m_DepthLevelOfMapNode)
		{
			map<M3DNObject*, MTreeNode*>::iterator temp = m_InstanceObjTable.find(data);
			if (temp != m_InstanceObjTable.end())
			{
				(*temp).second->m_pNObjList.erase(data);
				m_InstanceObjTable.erase(data);
			}
			m_InstanceObjTable.insert(make_pair(data, pNode));
			pNode->m_pNObjList.insert(data);
			return true;
		}
		else
		{
			for (auto temp : pNode->m_pChild)
			{
				if (CheckInstanceObj(temp, data))
				{
					return true;
				}
			}
			map<M3DNObject*, MTreeNode*>::iterator temp = m_InstanceObjTable.find(data);
			if (temp != m_InstanceObjTable.end())
			{
				(*temp).second->m_pNObjList.erase(data);
				m_InstanceObjTable.erase(data);
			}
			m_InstanceObjTable.insert(make_pair(data, pNode));
			pNode->m_pNObjList.insert(data);
			return true;
		}
	}
}

bool MTree::CheckInstanceObject(M3DNObject * data)
{
	// 기존것이던 새로운것이던 상관없음, 있으면 테이블에서 제거하고 걍 추가됨 //
	return CheckInstanceObj(m_pRootNode, data);
}

bool MTree::DeleteInstancObject(M3DNObject * data)
{
	map<M3DNObject*, MTreeNode*>::iterator iter = m_InstanceObjTable.find(data);
	if (iter == m_InstanceObjTable.end())
	{
		return false;
	}
	(*iter).second->m_pNObjList.erase((*iter).first);
	m_InstanceObjTable.erase((*iter).first);
	return true;
}


bool MTree::Frame()
{
	m_RenderNodeList.clear();
	m_RenderObjList.clear();
	return true;
}

bool MTree::Render()
{
	CheckNode(m_pRootNode);
	if(I_3DObjectMgr.m_InWorldFiled)	I_3DObjectMgr.m_InWorldFiled->Render();
	for (auto node : m_RenderNodeList)
	{
		node->MapRender();
	}
	for (auto node : m_RenderObjList)
	{
		node->PreRender();
	}
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
#if defined(DEBUG) || defined(_DEBUG)
	if (m_Tile)
	{
		if (g_isBoxRender)
		{
			m_Box.Render();
		}
	}
#endif
	for (auto obj : m_pObjList)
	{
		obj->Render();
	}
	for (auto obj : m_pNObjList)
	{
		obj->PreRender();
	}
	if (!m_Tile)
	{
		for (auto Child : m_pChild)
		{
			Child->Render();
		}
	}
	return true;
}

bool MTreeNode::MapRender()
{
	if (m_Tile)
	{
		m_Tile->Render();
	}
	else
	{
		for (auto Child : m_pChild)
		{
			Child->MapRender();
		}
	}
	return true;
}
