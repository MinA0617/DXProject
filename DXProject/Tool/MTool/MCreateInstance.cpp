#include "stdafx.h"
#include "MCreateInstance.h"


bool MCreateInstance::Create()
{
	if (!I_3DObjectMgr.m_InWorldFiled) return false;
	MRAY ray = MSelect::GetScreenRay();
	MTreeNode* pNode = MSelect::CheckNode(I_3DObjectMgr.m_pTree->m_pRootNode, &ray);
	D3DXVECTOR3 center;
	if (pNode)
	{
		if (MSelect::CheckTri(pNode, &ray, &center))
		{
			M3DInstanceModel* test = I_3DObjectMgr.GetInstanceModel(m_iID);
			if (test == nullptr) return false;
			int i = I_3DObjectMgr.AddInstanceObj(m_iID, 1);
			if (i != -1)
			{
				M3DInstance* target = I_3DObjectMgr.GetInstanceObj(m_iID, i);
				if(target) target->SetPosition(center);
				return true;
			}
		}
	}
	return false;
}

MCreateInstance::MCreateInstance()
{
	m_iID = -1;
}


MCreateInstance::~MCreateInstance()
{
}
