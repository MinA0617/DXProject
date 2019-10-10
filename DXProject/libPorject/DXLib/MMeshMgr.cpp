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

MMesh* MMeshMgr::operator [] (M_STR name)
{
	//// ȿ�� ���� ��� ���� ////
	for (ITOR temp = m_List.begin(); temp != m_List.end(); temp++)
	{
		if ((*temp).second->m_szName == name)
		{
			return (*temp).second;
		}
	}
	return 0;
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

//DWORD MMeshMgr::Load(M_STR NewFileName)
//{
//	if (NewFileName.empty()) return 0;
//	TCHAR Drive[MAX_PATH] = { 0, };
//	TCHAR Dir[MAX_PATH] = { 0, };
//	TCHAR Name[MAX_PATH] = { 0, };
//	TCHAR Ext[MAX_PATH] = { 0, };
//	_tsplitpath_s(NewFileName.c_str(), Drive, Dir, Name, Ext); // �н��� �̸��� Ȯ���� �����ֱ�
//	M_STR filename = Name; // �̸�����
//	filename += Ext; // Ȯ���� �߰��ؼ� ����
//
//	//// �̸� �ߺ��� ���� ////
//	for (ITOR itor = m_List.begin(); itor != m_List.end(); itor++)
//	{
//		MMesh* pData = (*itor).second;
//		if (pData->GetName() == filename)
//		{
//			return 0;
//		}
//	}
//	MMesh* pData = new MMesh;
//	if (pData->Load(NewFileName) == false)
//	{
//		delete pData;
//		return NULL;
//	}
//	pData->m_szName = filename;
//	m_List.insert(std::make_pair(m_iIndexCount, pData));
//	return m_iIndexCount++;
//	//// m_iCount 0���� ����д�(���з� ����) index ī��Ʈ �̱� ������ �ʱ�ȭ���� ////
//}

DWORD MMeshMgr::Add(MMesh* data, M_STR name)
{
	//// �̸� �ߺ��� ���� ////
	for (ITOR itor = m_List.begin(); itor != m_List.end(); itor++)
	{
		MMesh* pData = (*itor).second;
		if (pData->GetName() == name)
		{
			delete data;
			return 0;
		}
	}
	data->m_szName = name;
	m_List.insert(std::make_pair(m_iIndexCount, data));
	return m_iIndexCount++;
}