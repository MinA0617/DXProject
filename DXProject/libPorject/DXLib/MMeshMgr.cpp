#include "MMeshMgr.h"



MMeshMgr::MMeshMgr()
{
	m_iIndexCount = 1;
}


MMeshMgr::~MMeshMgr()
{
	Release();
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

MMesh * MMeshMgr::operator[](M_STR name)
{
	ITOR data = m_List.find(name);
	if (data != m_List.end())
	{
		return (*data).second;
	}
	return nullptr;
}

MMesh* MMeshMgr::Add(MMesh* data, M_STR name)
{
	for (ITOR data = m_List.begin(); data != m_List.end(); data++)
	{
		if ((*data).first == name)
		{
			return nullptr;
		}
	}
	m_List.insert(std::make_pair(name, data));
	return data;
}