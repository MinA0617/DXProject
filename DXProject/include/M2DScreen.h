#pragma once
#include "M2DObject.h"
class M2DScreen : public M2DObject
{
public:
	bool Render() override;
public:
	M2DScreen();
	~M2DScreen();
};

