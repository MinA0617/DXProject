#pragma once
#include"MLight.h"

class MLightMgr : public MSingleton<MLightMgr>
{
public:
	friend class MSingleton<MLightMgr>;
	MLight m_List;
private:
	MLightMgr();
public:
	~MLightMgr();
	bool Init();
	bool Frame();
	bool Render();
	bool Release();
};

#define I_LightMgr MLightMgr::GetInstance()
