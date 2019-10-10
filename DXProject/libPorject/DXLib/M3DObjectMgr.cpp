#include "M3DObjectMgr.h"

M3DObject * M3DObjectMgr::operator[](M_STR name)
{
	ITOR temp = m_ObjList.find(name);
	if (temp == m_ObjList.end())
	{
		return nullptr;
	}
	else
	{
		return (*temp).second;
	}
}

bool M3DObjectMgr::Add(M_STR name, M3DObject * data)
{
	for (ITOR temp = m_ObjList.begin(); temp != m_ObjList.end(); temp++)
	{
		if ((*temp).first == name)
		{
			return false;
		}
	}
	m_ObjList.insert(make_pair(name, data));
	return true;
}

bool M3DObjectMgr::Delete(M_STR name)
{
	ITOR temp = m_ObjList.find(name);
	if (temp == m_ObjList.end())
	{
		return false;
	}
	else
	{
		m_ObjList.erase(name);
		return true;
	}
}

bool M3DObjectMgr::Init()
{
	I_CameraMgr.Init();
	I_LightMgr.Init();
	return true;
}

bool M3DObjectMgr::Frame()
{
	I_CameraMgr.Frame();
	I_LightMgr.Frame();
	I_SkeletonMgr.Frame();
	for (ITOR temp = m_ObjList.begin(); temp != m_ObjList.end(); temp++)
	{
		(*temp).second->Frame();
	}
	return true;
}

bool M3DObjectMgr::Render()
{
	I_CameraMgr.Render();
	I_LightMgr.Render();
	I_SkeletonMgr.Render();
	for (ITOR temp = m_ObjList.begin(); temp != m_ObjList.end(); temp++)
	{
		(*temp).second->Render();
	}
	return true;
}

bool M3DObjectMgr::Release()
{
	I_CameraMgr.Release();
	I_LightMgr.Release();
	I_SkeletonMgr.Release();
	for (ITOR temp = m_ObjList.begin(); temp != m_ObjList.end(); temp++)
	{
		(*temp).second->Release();
		delete (*temp).second;
	}
	m_ObjList.clear();
	return true;
}

M3DObjectMgr::M3DObjectMgr()
{
}


M3DObjectMgr::~M3DObjectMgr()
{
}
