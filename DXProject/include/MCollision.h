#pragma once
#include "MStd.h"

class MCollision : public MSingleton<MCollision>
{

	friend class MSingleton<MCollision>;
private:
		MCollision();
public:
	static bool RectInPt(RECT rt, MPoint pt)
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
	static bool RectInRect(RECT rt1, RECT rt2)
	{
		//거리 구해서 한번 거르기

		if (GetDistance(MPoint(rt1.left, rt1.top), MPoint(rt1.right, rt1.bottom)) +
			GetDistance(MPoint(rt2.left, rt2.top), MPoint(rt2.right, rt2.bottom)) <
			GetDistance(GetCenter(rt1), GetCenter(rt2)))
		{
			return false;
		}
		if (true == RectInPt(rt1, MPoint(rt2.left, rt2.top)))
		{
			return true;
		}
		if (true == RectInPt(rt1, MPoint(rt2.right, rt2.top)))
		{
			return true;
		}
		if (true == RectInPt(rt1, MPoint(rt2.left, rt2.bottom)))
		{
			return true;
		}
		if (true == RectInPt(rt1, MPoint(rt2.right, rt2.bottom)))
		{
			return true;
		}
		return false;
	}
	static long RectInRect_R(RECT rt1, RECT rt2)
	{
		//거리 구해서 한번 거르기
		//방향과 부딪힌 거리값(롤백해야될 값) 리턴
		// MAKELONG으로 리턴
		// 하이 부분에 
		if (GetDistance(MPoint(rt1.left, rt1.top), MPoint(rt1.right, rt1.bottom)) +
			GetDistance(MPoint(rt2.left, rt2.top), MPoint(rt2.right, rt2.bottom)) <
			GetDistance(GetCenter(rt1), GetCenter(rt2)))
		{
			return false;
		}
		int direction; // 1,2,3,4를 플래그값(동서남북)으로 가진다.
		float distance; // 거리
		if (true == RectInPt(rt1, MPoint(rt2.left, rt2.top)))
		{
			if (rt1.right - rt2.left < rt2.top - rt1.bottom)
			{
				direction = 1;
				distance = rt1.right - rt2.left;
			}
			else
			{
				direction = 3;
				distance = rt2.top - rt1.bottom;
			}
			return MAKELONG(direction, distance);
		}
		if (true == RectInPt(rt1, MPoint(rt2.right, rt2.top)))
		{
			if (rt2.right - rt1.left < rt2.top - rt1.bottom)
			{
				direction = 2;
				distance = rt2.right - rt1.left;
			}
			else
			{
				direction = 3;
				distance = rt2.top - rt1.bottom;
			}
			return MAKELONG(direction, distance);
		}
		if (true == RectInPt(rt1, MPoint(rt2.left, rt2.bottom)))
		{
			if (rt1.right - rt2.left < rt1.top - rt2.bottom)
			{
				direction = 1;
				distance = rt1.right - rt2.left;
			}
			else
			{
				direction = 4;
				distance = rt1.top - rt2.bottom;
			}
			return MAKELONG(direction, distance);
		}
		if (true == RectInPt(rt1, MPoint(rt2.right, rt2.bottom)))
		{
			if (rt2.right - rt1.left < rt1.top - rt2.bottom)
			{
				direction = 2;
				distance = rt2.right - rt1.left;
			}
			else
			{
				direction = 4;
				distance = rt1.top - rt2.bottom;
			}
			return MAKELONG(direction, distance);
		}
		return false;
	}
	static float GetDistance(MPoint pos1, MPoint pos2)
	{
		float result = sqrt(pow(pos1.x - pos2.x, 2) + pow(pos1.y - pos2.y, 2));
		return result;
	}
	static MPoint GetCenter(RECT rt)
	{
		MPoint result = MPoint((rt.right - rt.left) / 2, (rt.top - rt.bottom) / 2);
		return result;
	}
public:
	virtual ~MCollision();
};

#define I_Collision MCollision::GetInstance()