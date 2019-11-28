#include "MFunction.h"

int MFunction::Seed = 0;

int MFunction::Rand_I(int max)
{
	srand(Seed++ + time(NULL));
	return (rand() % (max + 1));
}

int MFunction::Rand_I(int min, int max)
{
	srand(Seed++ + time(NULL));
	return (rand() % (max + 1)) + min;
}

int MFunction::Rand_PULMA()
{
	srand(Seed++ + time(NULL));
	int i = (rand() % 2);
	if (i == 0) return -1;
	else return 1;
}

MFunction::MFunction()
{
}


MFunction::~MFunction()
{
}
