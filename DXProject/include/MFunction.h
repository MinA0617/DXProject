#pragma once
#include "MStd.h"

class MFunction
{
public:
	static int Rand_I(int max);
	static int Rand_I(int min, int max);
public:
	MFunction();
	virtual ~MFunction();
};

