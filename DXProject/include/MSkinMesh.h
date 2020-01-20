#pragma once
#include "MMesh.h"

class MSkinMesh : public MMesh
{
public:
	ID3D11Buffer*		m_pSkinBuffer;
public:
	bool				SetSkeletonAndCreateData(MSkeleton* skt, vector<CVERTEX>& OldData);
public:
	bool Init() override;
	bool Render() override;
	bool Release() override;
	MSkinMesh();
	~MSkinMesh();
};

