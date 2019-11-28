#include "stdafx.h"
#include "MFunction.h"
#include "MCreateDeleteInstance.h"


bool MCreateDeleteInstance::Create()
{
	if (m_iID == -1) return false;
	if (!I_3DObjectMgr.m_InWorldFiled) return false;
	//MRAY ray = MSelect::GetScreenRay();
	//MSelect::PickGroundPosition(&center);
	//vector<MTreeNode*> list;
	//MSelect::CheckNode(I_3DObjectMgr.m_pTree->m_pRootNode, &ray);
	D3DXVECTOR3 center;
	if (MSelect::PickGroundPosition(&center))
	{
		vector<int>& modellist = (*list.find((*table.find(m_iID)).second)).second;
		int count = modellist.size();
		float realscl = m_fScale * (1 + (MFunction::Rand_PULMA()*(m_iScaleVariation * MFunction::Rand_I(100) * 0.0001)));
		float Pitch = DegreeToRandian(m_fRotationX) + (MFunction::Rand_PULMA() * (MPI * 2) * m_iRotationVariationX * 0.0001 * MFunction::Rand_I(100));
		float Yaw = DegreeToRandian(m_fRotationY) + (MFunction::Rand_PULMA() * (MPI * 2) * m_iRotationVariationY * 0.0001 * MFunction::Rand_I(100));
		float Roll = DegreeToRandian(m_fRotationZ) + (MFunction::Rand_PULMA() * (MPI * 2) * m_iRotationVariationZ * 0.0001 * MFunction::Rand_I(100));
		for (int i = 0; i < count; i++)
		{
			M3DInstanceModel* test = I_3DObjectMgr.GetInstanceModel(modellist[i]);
			if (test == nullptr) return false;
			D3DXVECTOR3 pos = center;
			D3DXQUATERNION	rot;
			D3DXQuaternionRotationYawPitchRoll(&rot, Yaw, Pitch, Roll);
			D3DXVECTOR3 scl(realscl, realscl, realscl);

			int result = I_3DObjectMgr.AddInstanceObj(modellist[i], &pos, &rot, &scl);
			if (result == -1) return false;
		}
	}
	return true;
}

bool MCreateDeleteInstance::Delete()
{
	DWORD modelid;
	DWORD instanceid;
	if (MSelect::PickObject(&modelid, &instanceid))
	{
		if (I_3DObjectMgr.GetInstanceModel(modelid)->DeleteInstanceObject(instanceid))
		{
			return true;
		}
	}
	return false;
}

MCreateDeleteInstance::MCreateDeleteInstance()
{
	m_iID = -1;
	m_fRotationX = 0;
	m_fRotationY = 0;
	m_fRotationZ = 0;
	m_fScale = 1;
	m_iRotationVariationX = 0;
	m_iScaleVariation = 0;
}


MCreateDeleteInstance::~MCreateDeleteInstance()
{
}
