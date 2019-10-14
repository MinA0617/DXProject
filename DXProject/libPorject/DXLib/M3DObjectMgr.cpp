#include "M3DObjectMgr.h"

M3DObject * M3DObjectMgr::operator[](M_STR name)
{
	for (ITOR temp = m_ObjList.begin(); temp != m_ObjList.end(); temp++)
	{
		if ((*temp).second->m_name == name)
		{
			return (*temp).second;
		}
	}
	return nullptr;
}

M3DObject * M3DObjectMgr::operator[](int index)
{
	ITOR temp = m_ObjList.find(index);
	if (temp != m_ObjList.end())
	{
		return (*temp).second;;
	}
	return nullptr;
}

bool M3DObjectMgr::Add(M3DObject * data)
{
	for (ITOR temp = m_ObjList.begin(); temp != m_ObjList.end(); temp++)
	{
		if ((*temp).second->m_name == data->m_name)
		{
			return false;
		}
	}
	m_ObjList.insert(make_pair(iCount++, data));
	return true;
}

bool M3DObjectMgr::Delete(M_STR name)
{
	for (ITOR temp = m_ObjList.begin(); temp != m_ObjList.end(); temp++)
	{
		if ((*temp).second->m_name == name)
		{
			delete (*temp).second;
			m_ObjList.erase((*temp).first);
			return true;
		}
	}
	return false;
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
	iCount = 0;
}


M3DObjectMgr::~M3DObjectMgr()
{
}
