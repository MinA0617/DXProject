#include "MSkeletonMgr.h"



MSkeleton* MSkeletonMgr::operator[](M_STR name)
{
	ITOR skt = m_SktList.find(name);
	if (skt == m_SktList.end())
	{
		return nullptr;
	}
	return (*skt).second;
}

MSkeleton* MSkeletonMgr::Add(MSkeleton* data, M_STR name)
{
	data->m_name = name;
	if (m_SktList.find(name) != m_SktList.end())
	{
		return nullptr;
	}
	m_SktList.insert(make_pair(name, data));
	return data;
}

bool MSkeletonMgr::Delete(M_STR name)
{
	ITOR temp = m_SktList.find(name);
	if (temp == m_SktList.end())
	{
		return false;
	}
	else
	{
		delete (*m_SktList.find(name)).second;
		m_SktList.erase(name);
		return true;
	}
}

bool MSkeletonMgr::Release()
{
	for (ITOR temp = m_SktList.begin(); temp != m_SktList.end(); temp++)
	{
		(*temp).second->Release();
		delete (*temp).second;
	}
	return true;
}

MSkeletonMgr::MSkeletonMgr()
{
	iCount = 0;
}


MSkeletonMgr::~MSkeletonMgr()
{
}
