#include "MSoundMgr.h"


MSoundMgr::MSoundMgr()
{
	m_fBgmVolume = 0;
	m_iNuMSound = 0;
}

MSoundMgr::~MSoundMgr()
{
}

bool MSoundMgr::Init()
{
	FMOD::System_Create(&m_pSystem);
	m_pSystem->init(5, FMOD_INIT_NORMAL, 0);
	return true;
	return 1;
}

bool MSoundMgr::Frame()
{
	return 1;
}

bool MSoundMgr::Render()
{
	return 1;
}

bool MSoundMgr::Release()
{
	for (ITOR itor = m_List.begin(); itor != m_List.end();)
	{
		MSound* pSound = (*itor).second;
		pSound->Release();
		delete pSound;
		itor = m_List.erase(itor);
	}
	m_List.clear();

	m_pSystem->close();
	m_pSystem->release();
	return true;
	return 1;
}

int MSoundMgr::Load(M_STR NewFileName)
{
	TCHAR Drive[MAX_PATH] = { 0, };
	TCHAR Dir[MAX_PATH] = { 0, };
	TCHAR Name[MAX_PATH] = { 0, };
	TCHAR Ext[MAX_PATH] = { 0, };
	_tsplitpath_s(NewFileName.c_str(), Drive, Dir, Name, Ext); // 패스와 이름과 확장자 끊어주기
	M_STR filename = Name; // 이름저장
	filename += Ext; // 확장자 추가해서 저장
	M_STR path = Drive; // path에는 
	path += Dir;
	path += filename;

	MSound* newfile = new MSound;
	std::string ret = std::string(path.begin(), path.end());

	//// 중복 찾기
	for (ITOR temp = m_List.begin(); temp != m_List.end(); temp++)
	{
		if ((*temp).second->m_szName == filename)
		{
			delete(newfile);
			return 0;
		}
	}
	//// 제대로 열리는지 확인/ 안되면 0리턴
	if (m_pSystem->createSound(ret.c_str(), FMOD_LOOP_NORMAL, 0, &newfile->m_pSound) != 0)
	{
		delete(newfile);
		return 0;
	}

	newfile->iIndex = m_iNuMSound;
	newfile->m_pSystem = m_pSystem;
	newfile->m_szName = filename;
	m_List.insert(std::make_pair(newfile->iIndex, newfile));
	m_iNuMSound++;
	return newfile->iIndex;
}

MSound* MSoundMgr::operator [] (int iIndex)
{
	for (ITOR temp = m_List.begin(); temp != m_List.end(); temp++)
	{
		if ((*temp).first == iIndex)
		{
			return (*temp).second;
		}
	}
	return 0;
}

MSound* MSoundMgr::operator [] (M_STR name)
{
	for (ITOR temp = m_List.begin(); temp != m_List.end(); temp++)
	{
		if ((*temp).second->m_szName == name)
		{
			return (*temp).second;
		}
	}
	return 0;
}
//template<class T>
//T* MSoundMgr<T>::operator [] (std::string name)
//{
//	return find(name);
//}
//
//template<class T>
//T* MSoundMgr<T>::find(int iNum)
//{
//	return m_list[iNum];
//}
//
//template<class T>
//T* MSoundMgr<T>::find(std::string name)
//{
//	return m_list[name];
//}