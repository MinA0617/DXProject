#include "MKeyAnimation.h"



MKeyAnimation::MKeyAnimation()
{
}


MKeyAnimation::~MKeyAnimation()
{
}

bool MKeyAnimation::Init()
{
	return true;
}

bool MKeyAnimation::Release()
{
	for (ITOR temp = m_List.begin(); temp != m_List.end(); temp++)
	{
		delete (*temp).second;
	}
	return true;
}
