#pragma once
#include "M3DModel.h"
#include "MSkeleton.h"

class MUnit : public M3DModel
{
public:
	MSkeleton*			m_Skeleton;
	ID3D11Buffer*		m_pSkinBuffer;
	vector<CVERTEX>		m_OldData;
public:
	bool		SetRCVertex(vector<MVERTEX>& mv, vector<SKINDATA>& ms);
	bool		SetSkeletonAndCreateData(MSkeleton* skt);
	bool		Init() override;
	bool		Frame() override;
	bool		Render() override;
	bool		Release() override;
public:
	MUnit();
	~MUnit();
};

