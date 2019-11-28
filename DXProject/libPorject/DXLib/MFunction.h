#pragma once
#include "MStd.h"

class MFunction
{
private:
	static int Seed;
public:
	static int Rand_I(int max);
	static int Rand_I(int min, int max);
	static int Rand_PULMA();
public:
	MFunction();
	virtual ~MFunction();
};

