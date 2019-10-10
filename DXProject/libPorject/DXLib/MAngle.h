#pragma once
#include "MStd.h"

class MAngle
{
public:
	MAngle();
	~MAngle();
	static float get_angle(MPoint pos1, MPoint pos2)
	{
		//pos1을 시작점으로 pos2의 각도를 구하는 공식
		float Width = pos2.x - pos1.x;
		float Height = pos2.y - pos1.y;
		float angle = atan2(abs(Height), abs(Width)) * 180 / 3.141592;;
		//float angle = atan2(Height, Width);

		if (Width < 0)
		{
			if (Height >= 0) // 2사분면
			{
				angle = 180 - angle;
			}
			else //3사분면
			{
				angle += 180;
			}
		}
		else
		{
			if (Height < 0) // 4사분면
			{
				angle = 360 - angle;
			}
		}
		return angle;
	};
};

