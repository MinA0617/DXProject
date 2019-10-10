#pragma once
#include "MSound.h"

class MSoundMgr : public MSingleton<MSoundMgr>
{
	friend class MSingleton<MSoundMgr>;
private:
	MSoundMgr();

	std::map<int, MSound*> m_List;
	typedef std::map<int, MSound*>::iterator ITOR;
public:
	virtual ~MSoundMgr();
public:
	MSound* operator [] (int iIndex);
	MSound* operator [] (M_STR name);
	FMOD::System* m_pSystem;
	float m_fBgmVolume;
	int	m_iNuMSound;
public:
	bool Init();
	bool Frame();
	bool Render();
	bool Release();
public:
	int Load(M_STR NewFileName);
	//int SE_Load(const char* NewFileName);
};




#define I_MSoundMgr MSoundMgr::GetInstance()