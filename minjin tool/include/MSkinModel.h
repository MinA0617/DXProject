#pragma once
#include "M3DModel.h"
#include "MSkeleton.h"

class MSkinModel : public M3DModel
{
public:
	MSkeleton*	m_Skeleton;
public:
	void		LinkParents(M3DObject* Parents);
	void		UnLinkParents();
public:
	bool		Init() override;
	bool		Frame() override;
	bool		Render() override;
	bool		Release() override;
public:
	MSkinModel();
	~MSkinModel();
};

