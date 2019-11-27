#include "MFunction.h"

int MFunction::Rand_I(int max)
{
	srand(time(NULL));
	return (rand() % max + 1);
}

int MFunction::Rand_I(int min, int max)
{
	srand(time(NULL));
	return (rand() % max + 1) + min;
}

MFunction::MFunction()
{
}


MFunction::~MFunction()
{
}
