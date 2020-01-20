#pragma once
#include "M3DInstanceModel.h"
#include "M3DInstanceSkinning.h"

struct BONEMATRIX
{
	D3DXMATRIX m[MAX_BONE];
};

class M3DInstanceSkinModel : public M3DInstanceModel
{
	// 버텍스 쉐이더가 달라야 한다.
	// 본행렬들을 넘길수 있어야한다.
public:
	vector<BONEMATRIX*>				m_BoneMatrixList;
	ID3D11Buffer*					m_InstanceBoneBuffer;
	bool							AddRenderObject(M3DInstanceSkinning* obj);
	bool							CreateInstanceBuffer() override;
	int								CreateInstanceObject(int count = 1);
public:
	bool							Frame();
	bool							Render();
public:
	M3DInstanceSkinModel();
	~M3DInstanceSkinModel();
};

