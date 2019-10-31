#pragma once
#include "MMesh.h"
#include "MSkeleton.h"

class MSkinMesh : public MMesh
{
public:
	ID3D11Buffer*		m_pSkinBuffer;
	vector<CVERTEX>		m_OldData;
public:
	bool				SetSkeletonAndCreateData(MSkeleton* skt);
public:
	bool Init() override;
	bool Render() override;
	bool Release() override;
	MSkinMesh();
	~MSkinMesh();
};

