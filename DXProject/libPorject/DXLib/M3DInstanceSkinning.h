#pragma once
#include "M3DInstance.h"
#include "MSkeleton.h"

class M3DInstanceSkinModel;
class M3DInstanceSkinning : public M3DNObject
{
	friend class M3DInstanceSkinModel;
public:
	MSkeleton*					m_pSkt;
	M3DInstanceSkinModel*		m_pModel;
public:
	bool						PreRender();
public:
	M3DInstanceSkinning(M3DInstanceSkinModel* target);
	~M3DInstanceSkinning();
};