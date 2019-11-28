#pragma once
#include "MStd.h"
#include "M3DBasic.h"

class M3DObject;
class MBoundingBox
{
public:
	D3DXVECTOR3		vPoint[8];
	bool			n_bCanOverlap;
	D3DXVECTOR3		vMin;
	D3DXVECTOR3		vMax;
	D3DXVECTOR3		vCenter;
	D3DXVECTOR3		vOldCenter;
	D3DXVECTOR3		vAxis[3];
	float			fOldExtent[3];
	float			fExtent[3];
	bool			Copy(MBoundingBox* target);
	bool			BuildForMinMax();
	bool			SelfUpdate();
	bool			Updata(D3DXVECTOR3* pos, D3DXQUATERNION* rot, D3DXVECTOR3* scl);
	bool			UpdataMaxY(float maxy);
	vector<MBoundingBox*> m_ChildList;
	//----------
#if defined(DEBUG) || defined(_DEBUG)
	ID3D11Buffer*		m_pConstantBuffer;
	bool Init();
	bool Render();
	bool Release();
#endif
	//----------
	MBoundingBox();
	~MBoundingBox();
};

