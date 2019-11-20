#include "stdafx.h"
#include "MPushPull.h"

void MPushPull::ModifyVertex(DWORD index, float distance)
{
	float dis = distance / m_fRadius;
	float power = cos(dis * 3.14 / 2) * m_fPower * g_fSeoundPerFrame * (-((float)m_bReversal - 0.5) * 2);
	m_pHM->m_VertexList[index].p.y += power;
}

bool MPushPull::SetRadius(float radius)
{
	if (radius <= 0) return false;
	m_fRadius = radius;
	return true;
}

bool MPushPull::SetPower(float power)
{
	if (power <= 0) return false;
	m_fPower = power;
	return true;
}

bool MPushPull::PushPull()
{
	if (!I_3DObjectMgr.m_InWorldFiled) return false;
	m_pHM = I_3DObjectMgr.m_InWorldFiled->ground;
	MRAY ray = MSelect::GetScreenRay();
	MTreeNode* pNode = MSelect::CheckNode(I_3DObjectMgr.m_pTree->m_pRootNode, &ray);
	D3DXVECTOR3 center;
	DWORD		index;
	int	XCount = m_pHM->m_iCount;
	if (pNode)
	{
		if (MSelect::CheckTri(pNode, &ray, &center, &index))
		{
			D3DXVECTOR2  center2d(center.x, center.z);
			vector<DWORD> Xlist;
			vector<DWORD> XZlist;
			vector<MVERTEX>& vertexlist = m_pHM->m_VertexList;
			{
				DWORD i = 0;
				while (1)
				{
					if (index + i >= vertexlist.size()) break;
					D3DXVECTOR3& vertex = vertexlist[index + i].p;
					D3DXVECTOR2 length(vertex.x, vertex.z);
					float distance = D3DXVec2Length(&(center2d - length));
					if (distance < m_fRadius)
					{
						ModifyVertex(index + i, distance);
						Xlist.push_back(index + i);
						XZlist.push_back(index + i);
					}
					else
					{
						break;
					}
					i++;
				}
				i = 1;
				while (1)
				{
					if (index < i) break;
					D3DXVECTOR3& vertex = vertexlist[index - i].p;
					D3DXVECTOR2 length(vertex.x, vertex.z);
					float distance = D3DXVec2Length(&(center2d - length));
					if (distance < m_fRadius)
					{
						ModifyVertex(index - i, distance);
						Xlist.push_back(index - i);
						XZlist.push_back(index - i);
					}
					else
					{
						break;
					}
					i++;
				}
				for (int j = 0; j < Xlist.size(); j++)
				{
					i = XCount;
					while (1)
					{
						if (Xlist[j] + i >= vertexlist.size()) break;
						D3DXVECTOR3& vertex = vertexlist[Xlist[j] + i].p;
						D3DXVECTOR2 length(vertex.x, vertex.z);
						float distance = D3DXVec2Length(&(center2d - length));
						if (distance < m_fRadius)
						{
							ModifyVertex(Xlist[j] + i, distance);
							XZlist.push_back(Xlist[j] + i);
						}
						else
						{
							break;
						}
						i = i + XCount;
					}
					i = XCount;
					while (1)
					{
						if (Xlist[j] < i) break;
						D3DXVECTOR3& vertex = vertexlist[Xlist[j] - i].p;
						D3DXVECTOR2 length(vertex.x, vertex.z);
						float distance = D3DXVec2Length(&(center2d - length));
						if (distance < m_fRadius)
						{
							ModifyVertex(Xlist[j] - i, distance);
							XZlist.push_back(Xlist[j] - i);
						}
						else
						{
							break;
						}
						i = i + XCount;
					}
				}
			}

			// Normal Update;

			set<DWORD> Facelist;
			for (int i = 0; i < XZlist.size(); i++)
			{
				// 업데이트 할 페이스를 찾는다.
				for (int j = 0; j < m_pHM->m_VertexFaceIndex[XZlist[i]].size(); j++)
				{
					set<DWORD>::iterator temp = Facelist.find(m_pHM->m_VertexFaceIndex[XZlist[i]][j]);
					if (temp == Facelist.end())
					{
						Facelist.insert(m_pHM->m_VertexFaceIndex[XZlist[i]][j]);
					}
				}
			}

			set<DWORD> Vertexlist;
			for (set<DWORD>::iterator temp = Facelist.begin(); temp != Facelist.end(); temp++)
			{
				m_pHM->m_FaceNormal[(*temp)] = m_pHM->ComputeFaceNormal(m_pHM->m_IndexList[(*temp) * 3 + 0],
					m_pHM->m_IndexList[(*temp) * 3 + 1],
					m_pHM->m_IndexList[(*temp) * 3 + 2]);
				for (int i = 0; i < 3; i++)
				{
					set<DWORD>::iterator itor = Vertexlist.find(m_pHM->m_IndexList[(*temp) * 3 + i]);
					if (itor == Vertexlist.end())
					{
						Vertexlist.insert(m_pHM->m_IndexList[(*temp) * 3 + i]);
					}
				}
			}

			for (set<DWORD>::iterator temp = Vertexlist.begin(); temp != Vertexlist.end(); temp++)
			{
				m_pHM->ComputeVertexNormal((*temp));
			}

			g_pImmediateContext->UpdateSubresource(m_pHM->m_pVertexBuffer, 0, 0, &m_pHM->m_VertexList.at(0), 0, 0);

			return true;
		}
	}
	return false;
}

MPushPull::MPushPull()
{
	m_fRadius = 100.0;
	m_fPower = 100.0;
	m_pHM = nullptr;
}


MPushPull::~MPushPull()
{
}
