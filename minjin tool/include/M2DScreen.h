#pragma once
#include "M2DObject.h"
class M2DScreen : public M2DObject
{
public:
	int		m_iWidth;
	int		m_iHeigth;
	bool	Render() override;
public:
	M2DScreen();
	~M2DScreen();
};

