#include "M3DInstance.h"
#include "M3DObjectMgr.h"

bool M3DInstance::PreRender()
{
	m_pModel->AddRenderObject(this);
	return true;
}

M3DInstance::M3DInstance(M3DInstanceModel * target)
{
	m_pModel = target;
}

M3DInstance::~M3DInstance()
{
}