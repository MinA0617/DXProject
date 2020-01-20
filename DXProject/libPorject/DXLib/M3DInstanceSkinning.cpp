#include "M3DInstanceSkinning.h"
#include "M3DInstanceSkinModel.h"

bool M3DInstanceSkinning::PreRender()
{
	m_pModel->AddRenderObject(this);
	return true;
}

M3DInstanceSkinning::M3DInstanceSkinning(M3DInstanceSkinModel * target)
{
	m_pModel = target;
}

M3DInstanceSkinning::~M3DInstanceSkinning()
{
}
