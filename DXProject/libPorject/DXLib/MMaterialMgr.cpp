#include "MMaterialMgr.h"




DWORD MMaterialMgr::CreateNewMaterial()
{
	MMaterial* newdata = new MMaterial;
	newdata->Init();
	m_List.push_back(newdata);
	m_iCount++;
	return m_iCount;
}

bool MMaterialMgr::Init()
{
	m_iCount = 0;
	MMaterial* head = nullptr;
	m_List.push_back(head);

	return true;
}

bool MMaterialMgr::Frame()
{
	for (int i = 1; i < m_List.size(); i++)
	{
		m_List[i]->Frame();
	}
	return true;
}

bool MMaterialMgr::Release()
{
	for (int i = 1; i < m_List.size(); i++)
	{
		m_List[i]->Release();
		delete m_List[i];
	}
	m_List.clear();
	return true;
}

MMaterialMgr::MMaterialMgr()
{
}


MMaterialMgr::~MMaterialMgr()
{
}

MMaterial * MMaterialMgr::operator[](DWORD index)
{
	if (index >= m_List.size())
	{
		return nullptr;
	}
	return m_List[index];;
}
