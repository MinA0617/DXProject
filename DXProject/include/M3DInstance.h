#pragma once
#include "M3DNObject.h"

class M3DInstanceModel;
class M3DInstance : public M3DNObject
{
	friend class M3DInstanceModel;
public:
	M3DInstanceModel*		m_pModel;
public:
	bool					PreRender();
public:
	M3DInstance(M3DInstanceModel* target);
	virtual ~M3DInstance();
};

