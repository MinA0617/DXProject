#include "MSelect.h"



D3DXVECTOR2 MSelect::GetScreenPoint()
{
	return D3DXVECTOR2(g_MousePos.x / g_rtWindowClient.right * 2, g_MousePos.y / g_rtWindowClient.bottom * 2);
}

MRAY MSelect::GetScreenRay()
{
	MRAY ray;
	D3DXMATRIX View = I_CameraMgr.m_MainCamera->m_matView;
	D3DXMATRIX Proj = I_CameraMgr.m_MainCamera->m_matProj;
	D3DXVECTOR3 v;
	POINT ptCursor;
	GetCursorPos(&ptCursor);
	ScreenToClient(g_hWnd, &ptCursor);
	//v.x = (((2.0f * ptCursor.x) / g_rtWindowClient.right)-1) / Proj._11;
	//v.y = -(((2.0f * ptCursor.y) / g_rtWindowClient.bottom)-1) / Proj._22;
	v.x = (((2.0f * g_MousePos.x) / g_rtWindowClient.right)) / Proj._11;
	v.y = (((2.0f * g_MousePos.y) / g_rtWindowClient.bottom)) / Proj._22;
	v.z = 1.0f;
	D3DXMatrixInverse(&View, NULL, &View);
	ray.vOrigin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	ray.vDirection = D3DXVECTOR3(v.x, v.y, v.z);
	D3DXVec3TransformCoord(&ray.vOrigin, &ray.vOrigin, &View);
	D3DXVec3TransformNormal(&ray.vDirection, &ray.vDirection, &View);
	D3DXVec3Normalize(&ray.vDirection, &ray.vDirection);
	return ray;
}

bool MSelect::CheckTri(MTreeNode * pNode, MRAY * ray, D3DXVECTOR3* minintersection, DWORD* index)
{
	M3DHeightMap* map = I_3DObjectMgr.m_InWorldFiled->ground;
	vector<MVERTEX>& vertexlist = I_3DObjectMgr.m_InWorldFiled->ground->m_VertexList;
	vector<DWORD>& indexlist = pNode->m_Tile->m_iIndex[0];
	int iCount = indexlist.size();

	bool result = false;
	float mindistance = 99999999;

	for (int i = 0; i < iCount; i = i + 3)
	{
		D3DXVECTOR3 intersection;
		D3DXVECTOR3 facenormal = map->ComputeFaceNormal(indexlist[i + 0], indexlist[i + 1], indexlist[i + 2]);
		if (I_Collision.TritoRay(&facenormal, &vertexlist[indexlist[i + 0]].p, &vertexlist[indexlist[i + 1]].p, &vertexlist[indexlist[i + 2]].p, ray, &intersection))
		{
			float distance = I_Collision.GetDistance(ray->vOrigin, intersection);
			if (mindistance > distance)
			{
				if (minintersection)
				{
					mindistance = distance;
					*minintersection = intersection;
					result = true;
					if (index)
					{
						// 인덱스를 넘기면 인접인덱스의 번호를 리턴한다.
						*index = indexlist[i];
					}
				}
				else
				{
					return true;
				}
			}
		}
	}
	return result;
}

void MSelect::CheckNode(MTreeNode* pNode, MRAY* ray, vector<MTreeNode*>& list)
{
	int m_DepthLevelOfMapNode = I_3DObjectMgr.m_pTree->m_DepthLevelOfMapNode;
	MTreeNode* minNode = nullptr;
	if (I_Collision.AABBtoRay(&pNode->m_Box, ray))
	{
		if (pNode->m_iDepth == m_DepthLevelOfMapNode)
		{
			list.push_back(pNode);
			return;
		}
		else
		{
			for (int i = 0; i < pNode->m_pChild.size(); i++)
			{
				CheckNode(pNode->m_pChild[i], ray, list);
			}
			return;
		}
	}
	else
	{
		return;
	}
}

void MSelect::CheckUPNode(MTreeNode * pNode, MRAY * ray, vector<MTreeNode*>& list)
{
	int m_DepthLevelOfMapNode = I_3DObjectMgr.m_pTree->m_DepthLevelOfMapNode;
	MTreeNode* minNode = nullptr;
	MBoundingBox box;
	box.Copy(&pNode->m_Box);
	box.UpdataMaxY(99999999);
	if (I_Collision.AABBtoRay(&box, ray))
	{
		list.push_back(pNode);
		if (pNode->m_iDepth == m_DepthLevelOfMapNode)
		{
			return;
		}
		else
		{
			for (int i = 0; i < pNode->m_pChild.size(); i++)
			{
				CheckUPNode(pNode->m_pChild[i], ray, list);
			}
			return;
		}
	}
	else
	{
		return;
	}
}


M3DNObject* MSelect::PickObject(DWORD* ModelID, DWORD* InstanceID, D3DXVECTOR3* intersection)
{
	M3DNObject* result = nullptr;
	vector<MTreeNode*> nodelist;
	MRAY ray = GetScreenRay();
	float mindistnace = 9999999999;
	if (CheckUPNodeList(nodelist))
	{
		for (int i = 0; i < nodelist.size(); i++)
		{
			for (auto obj : nodelist[i]->m_pNObjList)
			{
				D3DXVECTOR3 inter;
				if (MCollision::OBBtoRay(&obj->m_Box, &ray, &inter))
				{
					float distance = MCollision::GetDistance(ray.vOrigin, inter);
					if (distance < mindistnace)
					{
						result = obj;
						if (intersection) *intersection = inter;
					}
				}
			}
		}
		if (result == nullptr) return result;
		if (ModelID)
		{
			M3DInstance* temp = (M3DInstance*)&result;
			*ModelID = I_3DObjectMgr.GetInstanceModelID(temp->m_pModel->m_name);
			if (InstanceID)
			{
				int instcount = temp->m_pModel->m_iCount;
				for (int i = 0; i < instcount; i++)
				{
					if (result == temp->m_pModel->GetInstanceObject(i))
					{
						*InstanceID = i;
						return result;
					}
				}
			}
		}
	}
	return result;
}

MMapNode * MSelect::PickMapNode()
{
	MMapNode* returnNode = nullptr;
	vector<MTreeNode*> list;
	D3DXVECTOR3 minvec(9999999, 9999999, 9999999);
	D3DXVECTOR3 result(0, 0, 0);
	MRAY ray = GetScreenRay();
	if (CheckNodeList(list))
	{
		for (int i = 0; i < list.size(); i++)
		{
			if (CheckTri(list[i], &ray, &result))
			{
				float mindistance = I_Collision.GetDistance(ray.vOrigin, minvec);
				float distance = I_Collision.GetDistance(ray.vOrigin, result);
				if (mindistance > distance)
				{
					minvec = result;
					returnNode = list[i]->m_Tile;
				}
			}
		}
	}
	return returnNode;
}

bool MSelect::CheckNodeList(vector<MTreeNode*>& list)
{
	CheckNode(I_3DObjectMgr.m_pTree->m_pRootNode, &GetScreenRay(), list);
	if (list.size() == 0) { return false; }
	else { return true; }
}

bool MSelect::CheckUPNodeList(vector<MTreeNode*>& list)
{
	CheckUPNode(I_3DObjectMgr.m_pTree->m_pRootNode, &GetScreenRay(), list);
	if (list.size() == 0) { return false; }
	else { return true; }
}

bool MSelect::PickGroundPosition(D3DXVECTOR3* result, DWORD* index)
{
	bool bresult = false;
	float mindistance = -1;
	vector<MTreeNode*> list;
	D3DXVECTOR3 minvec(999999,999999,999999);
	DWORD		minindex = -1;
	MRAY ray = GetScreenRay();
	CheckNode(I_3DObjectMgr.m_pTree->m_pRootNode, &ray, list);
	if (list.size() != 0)
	{
		for (int i = 0; i < list.size(); i++)
		{
			if (CheckTri(list[i], &ray, &minvec, &minindex))
			{
				bresult = true;
				if (mindistance == -1)
				{
					mindistance = I_Collision.GetDistance(ray.vOrigin, minvec);
					if (index)
					{
						*index = minindex;
					}
					if (result)
					{
						*result = minvec;
					}
				}
				else
				{
					float distance = I_Collision.GetDistance(ray.vOrigin, minvec);
					if (mindistance > distance)
					{
						if (index)
						{
							*index = minindex;
						}
						if (result)
						{
							*result = minvec;
						}
					}
				}
			}
		}
	}
	return bresult;
}

MSelect::MSelect()
{
}


MSelect::~MSelect()
{
}
