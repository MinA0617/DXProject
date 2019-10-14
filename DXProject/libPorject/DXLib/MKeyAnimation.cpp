#include "MKeyAnimation.h"



MKeyAnimation::MKeyAnimation()
{
	m_fMaxTime = -1;
}


MKeyAnimation::~MKeyAnimation()
{
}

float MKeyAnimation::GetMaxTime()
{
	if (m_fMaxTime != -1) return m_fMaxTime;
	for (ITOR data = m_List.begin(); data != m_List.end(); data++)
	{
		float maxtime = (*data).second->GetMaxTime();
		m_fMaxTime = max(m_fMaxTime, maxtime);
	}
	return m_fMaxTime;
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
