#include "MTextureMgr.h"



MTextureMgr::MTextureMgr()
{
	m_iIndexCount = 1;
}


MTextureMgr::~MTextureMgr()
{
	Release();
}


MTexture* MTextureMgr::operator [] (int iIndex)
{
	MTexture* temp = (*(m_List.find(iIndex))).second;
	if (m_List.find(iIndex) == m_List.end())
	{
		return NULL;
	}
	else
	{
		return temp;
	}
}

MTexture* MTextureMgr::operator [] (M_STR name)
{
	//// 효율 낮음 사용 지양 ////
	for (ITOR temp = m_List.begin(); temp != m_List.end(); temp++)
	{
		if ((*temp).second->m_szName == name)
		{
			return (*temp).second;
		}
	}
	return 0;
}
bool MTextureMgr::Release()
{
	for (auto data : m_List)
	{
		data.second->Release();
		delete data.second;
	}
	m_List.clear();
	return true;
}

DWORD MTextureMgr::Load(M_STR NewFileName)
{
	if (NewFileName.empty()) return 0;
	TCHAR Drive[MAX_PATH] = { 0, };
	TCHAR Dir[MAX_PATH] = { 0, };
	TCHAR Name[MAX_PATH] = { 0, };
	TCHAR Ext[MAX_PATH] = { 0, };
	_tsplitpath_s(NewFileName.c_str(), Drive, Dir, Name, Ext); // 패스와 이름과 확장자 끊어주기
	M_STR filename = Name; // 이름저장
	filename += Ext; // 확장자 추가해서 저장

	//// 이름 중복시 생성된 개체 반환 ////
	for (ITOR itor = m_List.begin(); itor != m_List.end(); itor++)
	{
		MTexture* pData = (*itor).second;
		if (pData->GetName() == filename)
		{
			return (*itor).first;
		}
	}
	MTexture* pData = new MTexture;
	if (pData->Load(NewFileName) == false)
	{
		delete pData;
		return NULL;
	}
	pData->m_szName = filename;
	m_List.insert(std::make_pair(m_iIndexCount, pData));
	return m_iIndexCount++;
	//// m_iCount 0번은 비워둔다(실패로 간주) index 카운트 이기 때문에 초기화없음 ////
}