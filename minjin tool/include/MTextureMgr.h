#pragma once
#include "MTexture.h"

class MTextureMgr : public MSingleton<MTextureMgr>
{
public:
	friend class MSingleton<MTextureMgr>;
private:
	MTextureMgr();
private:
	map<int, MTexture*> m_List;
	int					m_iIndexCount;
public:
	typedef std::map<int, MTexture*>::iterator ITOR;
	MTexture*	operator [] (int iIndex);
	MTexture*	operator [] (M_STR name);
public:
	DWORD		Load(M_STR filename);
	bool		Release();
public:
	~MTextureMgr();
};

#define I_TextureMgr MTextureMgr::GetInstance()