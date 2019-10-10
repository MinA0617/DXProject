#include "MLightMgr.h"



MLightMgr::MLightMgr()
{
}


MLightMgr::~MLightMgr()
{
}

bool MLightMgr::Init()
{
	m_List.Init();
	return true;
}

bool MLightMgr::Frame()
{
	m_List.Frame();
	return true;
}

bool MLightMgr::Render()
{
	m_List.Render();
	return true;
}

bool MLightMgr::Release()
{
	m_List.Release();
	return true;
}
