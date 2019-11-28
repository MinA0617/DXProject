#include "MFiled.h"
#include "M3DObjectMgr.h"


bool MFiled::Set(M_STR name, M3DHeightMap* map)
{
	m_name = name;
	ground = map;
	m_Box = new MBoundingBox;
#if defined(DEBUG) || defined(_DEBUG)
	m_Box->Init();
#endif // DEBUG

	m_Box->vMin = D3DXVECTOR3(-(ground->m_iCount - 1) * ground->m_fLeafSize / 2, 0, -(ground->m_iCount - 1) * ground->m_fLeafSize / 2);
	m_Box->vMax = D3DXVECTOR3((ground->m_iCount - 1) * ground->m_fLeafSize / 2, ground->m_fYmax, (ground->m_iCount - 1) * ground->m_fLeafSize / 2);
	m_Box->fOldExtent[0] = (m_Box->vMax.x - m_Box->vMin.x) / 2;
	m_Box->fOldExtent[1] = (m_Box->vMax.y - m_Box->vMin.y) / 2;
	m_Box->fOldExtent[2] = (m_Box->vMax.z - m_Box->vMin.z) / 2;
	m_Box->vOldCenter = (m_Box->vMax + m_Box->vMin) / 2;
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
#if defined(DEBUG) || defined(_DEBUG)
	if (g_isBoxRender)
	{
		if (m_Box)m_Box->Render();
	}
#endif // DEBUG
	if (ground) ground->Render();
	return true;
}

bool MFiled::Release()
{
	SAFE_RELEASE(ground);
	return true;
}

MFiled::MFiled()
{
	ground = nullptr;
}


MFiled::~MFiled()
{
}
