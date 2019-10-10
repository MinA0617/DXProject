#pragma once
#include "MStd.h"

class MAngle
{
public:
	MAngle();
	~MAngle();
	static float get_angle(MPoint pos1, MPoint pos2)
	{
		//pos1�� ���������� pos2�� ������ ���ϴ� ����
		float Width = pos2.x - pos1.x;
		float Height = pos2.y - pos1.y;
		float angle = atan2(abs(Height), abs(Width)) * 180 / 3.141592;;
		//float angle = atan2(Height, Width);

		if (Width < 0)
		{
			if (Height >= 0) // 2��и�
			{
				angle = 180 - angle;
			}
			else //3��и�
			{
				angle += 180;
			}
		}
		else
		{
			if (Height < 0) // 4��и�
			{
				angle = 360 - angle;
			}
		}
		return angle;
	};
};

