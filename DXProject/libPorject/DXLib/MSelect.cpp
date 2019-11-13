#include "MSelect.h"



D3DXVECTOR2 MSelect::GetScreenPoint()
{
	return D3DXVECTOR2(g_MousePos.x / g_rtWindowClient.right * 2, g_MousePos.y / g_rtWindowClient.bottom * 2);
}

MRAY MSelect::GetScreenRay()
{
	MRAY ray;
	D3DXMATRIX Proj, View;
	D3DXMatrixTranspose(&Proj, &I_CameraMgr.m_MainCamera->m_matProj);
	D3DXMatrixTranspose(&View, &I_CameraMgr.m_MainCamera->m_matView);
	D3DXVECTOR3 v;
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

bool MSelect::CheckTri(MTreeNode * pNode, MRAY * ray, D3DXVECTOR3* minintersection)
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
			//
			//*minintersection = vertexlist[indexlist[i + 0]].p;
			//return true;
			//
			float distance = I_Collision.GetDistance(ray->vOrigin, intersection);
			if (mindistance > distance)
			{
				mindistance = distance;
				*minintersection = intersection;
				result = true;
			}
		}
	}
	return result;
}

MTreeNode* MSelect::CheckNode(MTreeNode* pNode, MRAY* ray)
{
	int m_DepthLevelOfMapNode = I_3DObjectMgr.m_pTree->m_DepthLevelOfMapNode;
	MTreeNode* minNode = nullptr;
	float mindistance = 999999999;
	if (I_Collision.AABBtoRay(&pNode->m_Box, ray))
	{
		if (pNode->m_iDepth == m_DepthLevelOfMapNode)
		{
			return pNode;
		}
		else
		{
			for (int i = 0; i < pNode->m_pChild.size(); i++)
			{
				MTreeNode* result = CheckNode(pNode->m_pChild[i], ray);
				if (result)
				{
					float distance = I_Collision.GetDistance(ray->vOrigin, pNode->m_Box.vCenter);
					if (mindistance > distance)
					{
						mindistance = distance;
						minNode = result;
					}
				}
			}
			return minNode;
		}
	}
	else
	{
		return nullptr;
	}
}


bool MSelect::PickObject(M3DObject * result)
{

	return true;
}

bool MSelect::PickGroundPosition(D3DXVECTOR3 * result)
{
	MTreeNode* pNode = CheckNode(I_3DObjectMgr.m_pTree->m_pRootNode, &GetScreenRay());
	if (pNode)
	{
		if (CheckTri(pNode, &GetScreenRay(), result))
		{
			return true;
		}
	}
	return false;
}

MSelect::MSelect()
{
}


MSelect::~MSelect()
{
}
