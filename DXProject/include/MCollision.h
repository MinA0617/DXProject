#pragma once
#include "MStd.h"
#include "MBoundingBox.h"

class MCollision : public MSingleton<MCollision>
{
	friend class MSingleton<MCollision>;
private:
	MCollision() {};
public:
	static bool		RectToPoint(RECT rt, MPoint pt)
	{
		//충돌시 트루를 리턴
		if (rt.left <= pt.x && rt.right >= pt.x)
		{
			if (rt.top >= pt.y && rt.bottom <= pt.y)
			{
				return true;
			}
		}
		return false;
	}
	static bool		RectToRect(RECT rt1, RECT rt2)
	{
		//거리 구해서 한번 거르기

		if (GetDistance(MPoint(rt1.left, rt1.top), MPoint(rt1.right, rt1.bottom)) +
			GetDistance(MPoint(rt2.left, rt2.top), MPoint(rt2.right, rt2.bottom)) <
			GetDistance(GetCenter(rt1), GetCenter(rt2)))
		{
			return false;
		}
		if (true == RectToPoint(rt1, MPoint(rt2.left, rt2.top)))
		{
			return true;
		}
		if (true == RectToPoint(rt1, MPoint(rt2.right, rt2.top)))
		{
			return true;
		}
		if (true == RectToPoint(rt1, MPoint(rt2.left, rt2.bottom)))
		{
			return true;
		}
		if (true == RectToPoint(rt1, MPoint(rt2.right, rt2.bottom)))
		{
			return true;
		}
		return false;
	}
	//static long		RectInRect_R(RECT rt1, RECT rt2)
	//{
	//	//거리 구해서 한번 거르기
	//	//방향과 부딪힌 거리값(롤백해야될 값) 리턴
	//	// MAKELONG으로 리턴
	//	// 하이 부분에 
	//	if (GetDistance(MPoint(rt1.left, rt1.top), MPoint(rt1.right, rt1.bottom)) +
	//		GetDistance(MPoint(rt2.left, rt2.top), MPoint(rt2.right, rt2.bottom)) <
	//		GetDistance(GetCenter(rt1), GetCenter(rt2)))
	//	{
	//		return false;
	//	}
	//	int direction; // 1,2,3,4를 플래그값(동서남북)으로 가진다.
	//	float distance; // 거리
	//	if (true == RectToPoint(rt1, MPoint(rt2.left, rt2.top)))
	//	{
	//		if (rt1.right - rt2.left < rt2.top - rt1.bottom)
	//		{
	//			direction = 1;
	//			distance = rt1.right - rt2.left;
	//		}
	//		else
	//		{
	//			direction = 3;
	//			distance = rt2.top - rt1.bottom;
	//		}
	//		return MAKELONG(direction, distance);
	//	}
	//	if (true == RectToPoint(rt1, MPoint(rt2.right, rt2.top)))
	//	{
	//		if (rt2.right - rt1.left < rt2.top - rt1.bottom)
	//		{
	//			direction = 2;
	//			distance = rt2.right - rt1.left;
	//		}
	//		else
	//		{
	//			direction = 3;
	//			distance = rt2.top - rt1.bottom;
	//		}
	//		return MAKELONG(direction, distance);
	//	}
	//	if (true == RectToPoint(rt1, MPoint(rt2.left, rt2.bottom)))
	//	{
	//		if (rt1.right - rt2.left < rt1.top - rt2.bottom)
	//		{
	//			direction = 1;
	//			distance = rt1.right - rt2.left;
	//		}
	//		else
	//		{
	//			direction = 4;
	//			distance = rt1.top - rt2.bottom;
	//		}
	//		return MAKELONG(direction, distance);
	//	}
	//	if (true == RectToPoint(rt1, MPoint(rt2.right, rt2.bottom)))
	//	{
	//		if (rt2.right - rt1.left < rt1.top - rt2.bottom)
	//		{
	//			direction = 2;
	//			distance = rt2.right - rt1.left;
	//		}
	//		else
	//		{
	//			direction = 4;
	//			distance = rt1.top - rt2.bottom;
	//		}
	//		return MAKELONG(direction, distance);
	//	}
	//	return false;
	//}
	static float	GetDistance(MPoint pos1, MPoint pos2)
	{
		float result = sqrt(pow(pos1.x - pos2.x, 2) + pow(pos1.y - pos2.y, 2));
		return result;
	}
	static MPoint	GetCenter(RECT rt)
	{
		MPoint result = MPoint((rt.right - rt.left) / 2, (rt.top - rt.bottom) / 2);
		return result;
	}
	static bool		OBBtoOBB(MBoundingBox* box1, MBoundingBox* box2)
	{

		D3DXVECTOR3 D = box2->vCenter - box1->vCenter;
		float C[3][3];
		float absC[3][3];
		float AD[3];
		float R1, R2, R;
		float R12;
		// C와 absC값 구하기
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				C[i][j] = D3DXVec3Dot(&box1->vAxis[i], &box2->vAxis[j]);
				absC[i][j] = (float)fabs(C[i][j]);
			}
		}
		// 1
		for (int i = 0; i < 3; i++)
		{
			AD[i] = D3DXVec3Dot(&box1->vAxis[i], &D);
			R = (float)fabs(AD[i]);
			R2 = box2->fExtent[0] * absC[i][0] + box2->fExtent[1] * absC[i][1] + box2->fExtent[2] * absC[i][2];
			R12 = box1->fExtent[i] + R2;
			if (R > R12) return false;
		}
		// 2
		for (int i = 0; i < 3; i++)
		{
			R = (float)fabs(D3DXVec3Dot(&box2->vAxis[i], &D));
			R1 = box1->fExtent[0] * absC[0][i] + box1->fExtent[1] * absC[1][i] + box1->fExtent[2] * absC[2][i];
			R12 = box2->fExtent[i] + R1;
			if (R > R12) return false;
		}
		// 1 * 2
		R = (float)fabs(AD[2] * C[1][0] - AD[1] * C[2][0]);
		R1 = box1->fExtent[1] * absC[2][0] + box1->fExtent[2] * absC[1][0];
		R2 = box2->fExtent[1] * absC[0][2] + box2->fExtent[2] * absC[0][1];
		R12 = R1 + R2;
		if (R > R12)return false;

		R = (float)fabs(AD[2] * C[1][1] - AD[1] * C[2][1]);
		R1 = box1->fExtent[1] * absC[2][1] + box1->fExtent[2] * absC[1][1];
		R2 = box2->fExtent[0] * absC[0][2] + box2->fExtent[2] * absC[0][0];
		R12 = R1 + R2;
		if (R > R12)return false;

		R = (float)fabs(AD[2] * C[1][2] - AD[1] * C[2][2]);
		R1 = box1->fExtent[1] * absC[2][2] + box1->fExtent[2] * absC[1][2];
		R2 = box2->fExtent[0] * absC[0][1] + box2->fExtent[1] * absC[0][0];
		R12 = R1 + R2;
		if (R > R12)return false;
		//
		R = (float)fabs(AD[0] * C[2][0] - AD[2] * C[0][0]);
		R1 = box1->fExtent[0] * absC[2][0] + box1->fExtent[2] * absC[0][0];
		R2 = box2->fExtent[1] * absC[1][2] + box2->fExtent[2] * absC[1][1];
		R12 = R1 + R2;
		if (R > R12)return false;

		R = (float)fabs(AD[0] * C[2][1] - AD[2] * C[0][1]);
		R1 = box1->fExtent[0] * absC[2][1] + box1->fExtent[2] * absC[0][1];
		R2 = box2->fExtent[0] * absC[1][2] + box2->fExtent[2] * absC[1][0];
		R12 = R1 + R2;
		if (R > R12)return false;

		R = (float)fabs(AD[0] * C[2][2] - AD[2] * C[0][2]);
		R1 = box1->fExtent[0] * absC[2][2] + box1->fExtent[2] * absC[0][2];
		R2 = box2->fExtent[0] * absC[1][1] + box2->fExtent[1] * absC[1][0];
		R12 = R1 + R2;
		if (R > R12)return false;
		//
		R = (float)fabs(AD[1] * C[0][0] - AD[0] * C[1][0]);
		R1 = box1->fExtent[0] * absC[1][0] + box1->fExtent[1] * absC[0][0];
		R2 = box2->fExtent[1] * absC[2][2] + box2->fExtent[2] * absC[2][1];
		R12 = R1 + R2;
		if (R > R12)return false;

		R = (float)fabs(AD[1] * C[0][1] - AD[0] * C[1][1]);
		R1 = box1->fExtent[0] * absC[1][1] + box1->fExtent[1] * absC[0][1];
		R2 = box2->fExtent[0] * absC[2][2] + box2->fExtent[2] * absC[2][0];
		R12 = R1 + R2;
		if (R > R12)return false;

		R = (float)fabs(AD[1] * C[0][2] - AD[0] * C[1][2]);
		R1 = box1->fExtent[0] * absC[1][2] + box1->fExtent[1] * absC[0][2];
		R2 = box2->fExtent[0] * absC[2][1] + box2->fExtent[1] * absC[2][0];
		R12 = R1 + R2;
		if (R > R12)return false;
		return true;
	}
public:
	virtual ~MCollision() {};
};

#define I_Collision MCollision::GetInstance()