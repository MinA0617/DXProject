#include "MCollision.h"

inline bool MCollision::RectToPoint(RECT rt, D3DXVECTOR2 pt)
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

inline bool MCollision::RectToRect(RECT rt1, RECT rt2)
{
	//거리 구해서 한번 거르기

	if (GetDistance(D3DXVECTOR2(rt1.left, rt1.top), D3DXVECTOR2(rt1.right, rt1.bottom)) +
		GetDistance(D3DXVECTOR2(rt2.left, rt2.top), D3DXVECTOR2(rt2.right, rt2.bottom)) <
		GetDistance(GetCenter(rt1), GetCenter(rt2)))
	{
		return false;
	}
	if (true == RectToPoint(rt1, D3DXVECTOR2(rt2.left, rt2.top)))
	{
		return true;
	}
	if (true == RectToPoint(rt1, D3DXVECTOR2(rt2.right, rt2.top)))
	{
		return true;
	}
	if (true == RectToPoint(rt1, D3DXVECTOR2(rt2.left, rt2.bottom)))
	{
		return true;
	}
	if (true == RectToPoint(rt1, D3DXVECTOR2(rt2.right, rt2.bottom)))
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

inline float MCollision::GetDistance(D3DXVECTOR2 pos1, D3DXVECTOR2 pos2)
{
	float result = sqrt(pow(pos1.x - pos2.x, 2) + pow(pos1.y - pos2.y, 2));
	return result;
}

float MCollision::GetDistance(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2)
{
	D3DXVECTOR3 length = pos2 - pos1;
	return D3DXVec3Length(&length);
}

inline D3DXVECTOR2 MCollision::GetCenter(RECT rt)
{
	D3DXVECTOR2 result = D3DXVECTOR2((rt.right - rt.left) / 2, (rt.top - rt.bottom) / 2);
	return result;
}

inline bool MCollision::OBBtoOBB(MBoundingBox * box1, MBoundingBox * box2)
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

bool MCollision::AABBtoRay(MBoundingBox * box, MRAY * ray, D3DXVECTOR3* intersection)
{
	float t_min = 0;
	float t_max = 999999.0f;
	for (int i = 0; i < 3; i++)
	{
		// 축에 평행할 경우
		if (abs(ray->vDirection[i]) < 0.0001f)
		{
			if (ray->vOrigin[i] < box->vMin[i] || ray->vOrigin[i] > box->vMax[i])
				return false;
		}
		else
		{
			float denom = 1.0f / ray->vDirection[i];
			float t1 = (-ray->vOrigin[i] + box->vMin[i]) * denom;
			float t2 = (-ray->vOrigin[i] + box->vMax[i]) * denom;
			if (t1 > t2)
			{
				swap(t1, t2);
			}
			t_min = max(t_min, t1);
			t_max = min(t_max, t2);
			if (t_min > t_max)
				return false;
		}
	}
	if (intersection)ray->vOrigin + ray->vDirection* t_min;
	//m_vIntersection = ray->vOrigin + ray->vDirection* t_min;
	return true;
}

inline bool MCollision::OBBtoRay(MBoundingBox* box, MRAY * ray, D3DXVECTOR3* intersection)
{
	float t_min = -999999.0f;
	float t_max = 999999.0f;

	D3DXVECTOR3 diff = box->vCenter - ray->vOrigin;

	for (int v = 0; v < 3; v++)
	{
		float f = D3DXVec3Dot(&box->vAxis[v], &ray->vDirection);
		float s = D3DXVec3Dot(&box->vAxis[v], &diff);

		// 축에 평행할 경우
		if (abs(ray->vDirection[v]) < 0.0001f)
		{
			if (-s - box->fExtent[v] > 0 || -s + box->fExtent[v] > 0)
				return false;
		}
		else
		{
			float t1 = (s - box->fExtent[v]) / f;
			float t2 = (s + box->fExtent[v]) / f;
			if (t1 > t2)
			{
				swap(t1, t2);
			}
			t_min = max(t_min, t1);
			t_max = min(t_max, t2);
			if (t_min > t_max)
				return false;
		}
		if (intersection)ray->vOrigin + ray->vDirection* t_min;
	}
	return true;
}

bool MCollision::TritoRay(D3DXVECTOR3* normal, D3DXVECTOR3* v0, D3DXVECTOR3* v1, D3DXVECTOR3* v2, MRAY* ray, D3DXVECTOR3* intersection)
{
	D3DXVECTOR3 vDirection = (ray->vDirection * 10000);
	float D = D3DXVec3Dot(normal, &vDirection);
	float a0 = D3DXVec3Dot(normal, &(*v0 - ray->vOrigin));
	float fT = a0 / D;
	if (fT < 0.0f || fT > 1.0f)
	{
		return false;
	}
	D3DXVECTOR3 vIntersection = ray->vOrigin + vDirection * fT;

	D3DXVECTOR3 e0, e1, iInter, vNormal;
	e0 = *v2 - *v1;
	e1 = *v0 - *v1;
	iInter = vIntersection - *v1;
	D3DXVec3Cross(&vNormal, &e0, &iInter);
	D3DXVec3Normalize(&vNormal, &vNormal);
	float fDot = D3DXVec3Dot(normal, &vNormal);
	if (fDot < 0.0f) return false;

	D3DXVec3Cross(&vNormal, &iInter, &e1);
	D3DXVec3Normalize(&vNormal, &vNormal);
	fDot = D3DXVec3Dot(normal, &vNormal);
	if (fDot < 0.0f) return false;

	e0 = *v0 - *v2;
	e1 = *v1 - *v2;
	iInter = vIntersection - *v2;
	D3DXVec3Cross(&vNormal, &e0, &iInter);
	D3DXVec3Normalize(&vNormal, &vNormal);
	fDot = D3DXVec3Dot(normal, &vNormal);
	if (fDot < 0.0f) return false;

	D3DXVec3Cross(&vNormal, &iInter, &e1);
	D3DXVec3Normalize(&vNormal, &vNormal);
	fDot = D3DXVec3Dot(normal, &vNormal);
	if (fDot < 0.0f) return false;

	*intersection = vIntersection;
	return true;
}
