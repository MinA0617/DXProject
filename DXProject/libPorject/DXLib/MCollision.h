#pragma once
#include "MStd.h"
#include "MBoundingBox.h"

struct MRAY
{
	float			fExtent;
	D3DXVECTOR3		vOrigin;
	D3DXVECTOR3		vDirection;
	MRAY()
	{
		//  fExtent > 0 : 세그먼트로 사용 
		fExtent = -1;
	}
};

class MCollision : public MSingleton<MCollision>
{
	friend class MSingleton<MCollision>;
private:
	MCollision() {};
public:
	static bool			RectToPoint(RECT rt, D3DXVECTOR2 pt);
	static bool			RectToRect(RECT rt1, RECT rt2);
	static float		GetDistance(D3DXVECTOR2 pos1, D3DXVECTOR2 pos2);
	static float		GetDistance(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2);
	static D3DXVECTOR2	GetCenter(RECT rt);
	static bool			OBBtoOBB(MBoundingBox* box1, MBoundingBox* box2);
	static bool			AABBtoRay(MBoundingBox* box, MRAY* ray, D3DXVECTOR3* intersection = nullptr);
	static bool			OBBtoRay(MBoundingBox* box, MRAY* ray, D3DXVECTOR3* intersection = nullptr);
	static bool			TritoRay(D3DXVECTOR3* normal, D3DXVECTOR3* v0, D3DXVECTOR3* v1, D3DXVECTOR3* v2, MRAY* ray, D3DXVECTOR3* intersection = nullptr);
public:
	virtual ~MCollision() {};
};

#define I_Collision MCollision::GetInstance()