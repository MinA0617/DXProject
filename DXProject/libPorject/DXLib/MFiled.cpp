#include "MFiled.h"
#include "M3DObjectMgr.h"


bool MFiled::Set(M_STR name)
{
	ground = (M3DHeightMap*)I_3DObjectMgr.find(name);
	if (!ground) return false;
	MBoundingBox* box = new MBoundingBox;
	box->Init();
	box->m_pTarget = this;
	box->vMin = D3DXVECTOR3(-ground->m_iXCount * ground->m_fLeafSize / 2, 0, -ground->m_iZCount * ground->m_fLeafSize / 2);
	box->vMax = D3DXVECTOR3(ground->m_iXCount * ground->m_fLeafSize / 2, ground->m_fYmax, ground->m_iZCount * ground->m_fLeafSize / 2);
	box->fOldExtent[0] = (box->vMax.x - box->vMin.x) / 2;
	box->fOldExtent[1] = (box->vMax.y - box->vMin.y) / 2;
	box->fOldExtent[2] = (box->vMax.z - box->vMin.z) / 2;
	box->vOldCenter = (box->vMax + box->vMin) / 2;
	m_BoxList.push_back(box);
	CreateConstantBuffer();
	return true;
}

bool MFiled::Init()
{
	return true;
}

bool MFiled::Frame()
{
	PreFrame();
	if (ground) ground->Frame();
	return true;
}

bool MFiled::Render()
{
	for (auto box : m_BoxList)
	{
		box->Render();
	}
	if (ground) ground->Render();
	return true;
}

bool MFiled::Release()
{
	return true;
}

MFiled::MFiled()
{
	ground = nullptr;
}


MFiled::~MFiled()
{
}
