#include "stdafx.h"
#include "MPushPull.h"

void MPushPull::ModifyVertex(DWORD index, float distance)
{
	float dis = distance / m_fRadius;
	float power = cos(dis * 3.14 / 2) * m_fPower * g_fSeoundPerFrame * (-((float)m_bReversal - 0.5) * 2);
	if (m_pHM->m_VertexList[index].p.y + power < 0) { return; }
	m_pHM->m_VertexList[index].p.y += power;
}

void MPushPull::SetNonRender()
{
	m_pCircle->SetLocalScale(D3DXVECTOR3(0, 0, 0));
	m_pCircle->Frame();
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
	//if(MSelect::PickGroundPosition&(m_Center,&index))
	/*MRAY ray = MSelect::GetScreenRay();*/
	//MTreeNode* pNode = MSelect::CheckNode(I_3DObjectMgr.m_pTree->m_pRootNode, &ray);
	DWORD index = -1;
	int	XCount = m_pHM->m_iCount;
	if (MSelect::PickGroundPosition(&m_Center, &index))
	{
		D3DXVECTOR2  center2d(m_Center.x, m_Center.z);
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
		g_pImmediateContext->UpdateSubresource(m_pHM->m_pVertexBuffer, 0, 0, &m_pHM->m_VertexList.at(0), 0, 0);
	}
	//if (pNode)
	//{
	//	if (MSelect::CheckTri(pNode, &ray, &m_Center, &index))
	//	{
	//		D3DXVECTOR2  center2d(m_Center.x, m_Center.z);
	//		vector<DWORD> Xlist;
	//		vector<DWORD> XZlist;
	//		vector<MVERTEX>& vertexlist = m_pHM->m_VertexList;
	//		{
	//			DWORD i = 0;
	//			while (1)
	//			{
	//				if (index + i >= vertexlist.size()) break;
	//				D3DXVECTOR3& vertex = vertexlist[index + i].p;
	//				D3DXVECTOR2 length(vertex.x, vertex.z);
	//				float distance = D3DXVec2Length(&(center2d - length));
	//				if (distance < m_fRadius)
	//				{
	//					ModifyVertex(index + i, distance);
	//					Xlist.push_back(index + i);
	//					XZlist.push_back(index + i);
	//				}
	//				else
	//				{
	//					break;
	//				}
	//				i++;
	//			}
	//			i = 1;
	//			while (1)
	//			{
	//				if (index < i) break;
	//				D3DXVECTOR3& vertex = vertexlist[index - i].p;
	//				D3DXVECTOR2 length(vertex.x, vertex.z);
	//				float distance = D3DXVec2Length(&(center2d - length));
	//				if (distance < m_fRadius)
	//				{
	//					ModifyVertex(index - i, distance);
	//					Xlist.push_back(index - i);
	//					XZlist.push_back(index - i);
	//				}
	//				else
	//				{
	//					break;
	//				}
	//				i++;
	//			}
	//			for (int j = 0; j < Xlist.size(); j++)
	//			{
	//				i = XCount;
	//				while (1)
	//				{
	//					if (Xlist[j] + i >= vertexlist.size()) break;
	//					D3DXVECTOR3& vertex = vertexlist[Xlist[j] + i].p;
	//					D3DXVECTOR2 length(vertex.x, vertex.z);
	//					float distance = D3DXVec2Length(&(center2d - length));
	//					if (distance < m_fRadius)
	//					{
	//						ModifyVertex(Xlist[j] + i, distance);
	//						XZlist.push_back(Xlist[j] + i);
	//					}
	//					else
	//					{
	//						break;
	//					}
	//					i = i + XCount;
	//				}
	//				i = XCount;
	//				while (1)
	//				{
	//					if (Xlist[j] < i) break;
	//					D3DXVECTOR3& vertex = vertexlist[Xlist[j] - i].p;
	//					D3DXVECTOR2 length(vertex.x, vertex.z);
	//					float distance = D3DXVec2Length(&(center2d - length));
	//					if (distance < m_fRadius)
	//					{
	//						ModifyVertex(Xlist[j] - i, distance);
	//						XZlist.push_back(Xlist[j] - i);
	//					}
	//					else
	//					{
	//						break;
	//					}
	//					i = i + XCount;
	//				}
	//			}
	//		}

	//		// Normal Update;

	//		for (int i = 0; i < XZlist.size(); i++)
	//		{
	//			// 업데이트 할 페이스를 찾는다.
	//			for (int j = 0; j < m_pHM->m_VertexFaceIndex[XZlist[i]].size(); j++)
	//			{
	//				set<DWORD>::iterator temp = m_Facelist.find(m_pHM->m_VertexFaceIndex[XZlist[i]][j]);
	//				if (temp == m_Facelist.end())
	//				{
	//					m_Facelist.insert(m_pHM->m_VertexFaceIndex[XZlist[i]][j]);
	//				}
	//			}
	//		}

	//		//set<DWORD> Vertexlist;
	//		//for (set<DWORD>::iterator temp = m_Facelist.begin(); temp != m_Facelist.end(); temp++)
	//		//{
	//		//	m_pHM->m_FaceNormal[(*temp)] = m_pHM->ComputeFaceNormal(m_pHM->m_IndexList[(*temp) * 3 + 0],
	//		//		m_pHM->m_IndexList[(*temp) * 3 + 1],
	//		//		m_pHM->m_IndexList[(*temp) * 3 + 2]);
	//		//	for (int i = 0; i < 3; i++)
	//		//	{
	//		//		set<DWORD>::iterator itor = Vertexlist.find(m_pHM->m_IndexList[(*temp) * 3 + i]);
	//		//		if (itor == Vertexlist.end())
	//		//		{
	//		//			Vertexlist.insert(m_pHM->m_IndexList[(*temp) * 3 + i]);
	//		//		}
	//		//	}
	//		//}

	//		//for (set<DWORD>::iterator temp = Vertexlist.begin(); temp != Vertexlist.end(); temp++)
	//		//{
	//		//	m_pHM->ComputeVertexNormal((*temp));
	//		//}

	//		g_pImmediateContext->UpdateSubresource(m_pHM->m_pVertexBuffer, 0, 0, &m_pHM->m_VertexList.at(0), 0, 0);

	//		return true;
	//	}
	//}
	return false;
}

bool MPushPull::Update()
{
	set<DWORD> Vertexlist;
	for (set<DWORD>::iterator temp = m_Facelist.begin(); temp != m_Facelist.end(); temp++)
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
	m_Facelist.clear();

	I_3DObjectMgr.m_pTree->SetMaxY(I_3DObjectMgr.m_pTree->m_pRootNode, m_pHM);
	return true;
}

bool MPushPull::Init()
{
	m_pCircle = new M3DSpline;
	m_pCircle->Init();
	m_pCircle->SetLocalScale(D3DXVECTOR3(0, 0, 0));
	m_pCircle->Frame();
	m_pCircle->m_name = L"g_Circle";
	I_3DObjectMgr.Add(m_pCircle);

	vector<SPLINE_VERTEX> vertice;
	vector<DWORD>	index;

	int count = 32;

	for (int i = 0; i <= count; i++)
	{
		SPLINE_VERTEX data;
		data.p = D3DXVECTOR3(sin(MPI * i / count*2), 0, cos(MPI * i / count*2));
		vertice.push_back(data);
	}
	for (int i = 0; i < count; i++)
	{
		index.push_back(i);
		index.push_back(i + 1);
	}
	SPLINE_VERTEX start;
	SPLINE_VERTEX end;
	start.p = D3DXVECTOR3(0, 0, 0);
	end.p = D3DXVECTOR3(0, 1, 0);
	vertice.push_back(start);
	vertice.push_back(end);
	index.push_back(count + 1);
	index.push_back(count + 2);
	m_pCircle->Create(vertice, index);
	return true;
}

bool MPushPull::Release()
{
	SAFE_RELEASE(m_pCircle);
	return true;
}

bool MPushPull::Frame()
{
	if (!I_3DObjectMgr.m_InWorldFiled) return false;
	m_pHM = I_3DObjectMgr.m_InWorldFiled->ground;
	if (MSelect::PickGroundPosition(&m_Center))
	{
		m_pCircle->SetLocalPosition(m_Center + D3DXVECTOR3(0, 1, 0));
		m_pCircle->SetLocalScale(D3DXVECTOR3(m_fRadius, m_fRadius, m_fRadius));
		m_pCircle->Frame();
		return true;
	}
	//MRAY ray = MSelect::GetScreenRay();
	//MTreeNode* pNode = MSelect::CheckNode(I_3DObjectMgr.m_pTree->m_pRootNode, &ray);
	//DWORD		index;
	//int	XCount = m_pHM->m_iCount;
	//if (pNode)
	//{
	//	if (MSelect::CheckTri(pNode, &ray, &m_Center, &index))
	//	{
	//		m_pCircle->SetLocalPosition(m_Center + D3DXVECTOR3(0, 1, 0));
	//		m_pCircle->SetLocalScale(D3DXVECTOR3(m_fRadius, m_fRadius, m_fRadius));
	//		m_pCircle->Frame();
	//		return true;
	//	}
	//}
	m_pCircle->SetLocalScale(D3DXVECTOR3(0, 0, 0));
	m_pCircle->Frame();
	return true;
}

MPushPull::MPushPull()
{
	m_pCircle = nullptr;
	m_fRadius = 100.0;
	m_fPower = 100.0;
	m_pHM = nullptr;
	m_fTimer = 1.0f;
}


MPushPull::~MPushPull()
{
}
