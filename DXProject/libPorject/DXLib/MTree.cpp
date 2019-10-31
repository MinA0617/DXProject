#include "MTree.h"



bool MTree::AddObject(M3DObject * pObj)
{
	if (CheckRoot(pObj))
	{
	
	}
	return false;
}

bool MTree::Build(float fX, float fY, float fZ, float fMinSize)
{
	m_pRootNode = CreateNode(-fX / 2, -fY / 2, -fZ / 2, fX / 2, fY / 2, fZ / 2);
	m_fMinDivideSize = fMinSize;
	return BuildTree(m_pRootNode);
}

bool  MTree::BuildTree(MNode* pNode)
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

MNode*	MTree::CreateNode(float fMinX, float fMinY, float fMinZ, float fMaxX, float fMaxY, float fMaxZ, MNode* pParentNode)
{
	MNode* pNode = new MNode;
	pNode->m_pChild.reserve(0);

	pNode->m_Box.vMin.x = fMinX;
	pNode->m_Box.vMin.y = fMinY;
	pNode->m_Box.vMin.z = fMinZ;

	pNode->m_Box.vMax.x = fMaxX;
	pNode->m_Box.vMax.y = fMaxY;
	pNode->m_Box.vMax.z = fMaxZ;

	pNode->m_Box.vCenter.x = (fMinX + fMaxX) / 2;
	pNode->m_Box.vCenter.y = (fMinY + fMaxY) / 2;
	pNode->m_Box.vCenter.z = (fMinZ + fMaxZ) / 2;

	pNode->m_Box.vAxis[0] = D3DXVECTOR3(1, 0, 0);
	pNode->m_Box.vAxis[1] = D3DXVECTOR3(0, 1, 0);
	pNode->m_Box.vAxis[2] = D3DXVECTOR3(0, 0, 1);

	pNode->m_Box.fOldExtent[0] = (fMaxX - fMinX) / 2;
	pNode->m_Box.fOldExtent[1] = (fMaxX - fMinX) / 2;
	pNode->m_Box.fOldExtent[2] = (fMaxX - fMinX) / 2;


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

bool MTree::DeleteNode(MNode * pNode)
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

bool MTree::CheckNode(MNode pNode, M3DObject * pObj)
{
	//D3DVECTOR point[8];
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

bool MTree::SubDivide(MNode* pNode)
{
	float fx = (pNode->m_Box.vMax.x - pNode->m_Box.vMin.x) / 2;
	float fy = (pNode->m_Box.vMax.y - pNode->m_Box.vMin.y) / 2;
	float fz = (pNode->m_Box.vMax.z - pNode->m_Box.vMin.z) / 2;
	if (m_fMinDivideSize >= fx && m_fMinDivideSize >= fy && m_fMinDivideSize >= fz)
	{
		return false;
	}
	pNode->m_pChild.reserve(0);
	MBoundingBox pb = pNode->m_Box;
	float x[3] = { pb.vMin.x, pb.vMin.x + fx, pb.vMax.x };
	float y[3] = { pb.vMin.y, pb.vMin.y + fy, pb.vMax.y };
	float z[3] = { pb.vMin.z, pb.vMin.z + fz, pb.vMax.z };
	MNode* pChildNode[8];
	pChildNode[0] = CreateNode(x[0], y[0], z[0], x[1], y[1], z[1], pNode);
	pChildNode[1] = CreateNode(x[1], y[0], z[0], x[2], y[1], z[1], pNode);
	pChildNode[2] = CreateNode(x[0], y[1], z[0], x[1], y[2], z[1], pNode);
	pChildNode[3] = CreateNode(x[1], y[1], z[0], x[2], y[2], z[1], pNode);
	pChildNode[4] = CreateNode(x[0], y[0], z[1], x[1], y[1], z[0], pNode);
	pChildNode[5] = CreateNode(x[1], y[0], z[1], x[2], y[1], z[0], pNode);
	pChildNode[6] = CreateNode(x[0], y[1], z[1], x[1], y[2], z[0], pNode);
	pChildNode[7] = CreateNode(x[1], y[1], z[1], x[2], y[2], z[0], pNode);
	for (int i = 0; i < 8; i++)
	{
		pNode->m_pChild.push_back(pChildNode[i]);
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
}


MTree::~MTree()
{
}
