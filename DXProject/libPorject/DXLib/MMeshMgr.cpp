#include "MMeshMgr.h"



MMeshMgr::MMeshMgr()
{
	m_iIndexCount = 1;
}


MMeshMgr::~MMeshMgr()
{
	Release();
}

MMesh* MMeshMgr::operator [] (int iIndex)
{
	MMesh* temp = (*(m_List.find(iIndex))).second;
	if (m_List.find(iIndex) == m_List.end())
	{
		return NULL;
	}
	else
	{
		return temp;
	}
}

bool MMeshMgr::Release()
{
	for (auto data : m_List)
	{
		data.second->Release();
		delete data.second;

	}
	m_List.clear();
	return true;
}

DWORD MMeshMgr::Add(MMesh* data)
{
	m_List.insert(std::make_pair(m_iIndexCount, data));
	return m_iIndexCount++;
}