#include "MSkeletonMgr.h"



MSkeleton* MSkeletonMgr::operator[](int index)
{
	if (m_ObjList.find(index) != m_ObjList.end())
	{
		return (*m_ObjList.find(index)).second;
	}
	return nullptr;
}

int MSkeletonMgr::Add(MSkeleton* data)
{
	m_ObjList.insert(make_pair(iCount, data));
	return iCount++;
}

bool MSkeletonMgr::Delete(int index)
{
	ITOR temp = m_ObjList.find(index);
	if (temp == m_ObjList.end())
	{
		return false;
	}
	else
	{
		delete (*m_ObjList.find(index)).second;
		m_ObjList.erase(index);
		return true;
	}
}

bool MSkeletonMgr::Frame()
{
	for (ITOR temp = m_ObjList.begin(); temp != m_ObjList.end(); temp++)
	{
		(*temp).second->Frame();
	}
	return true;
}

bool MSkeletonMgr::Render()
{
	for (ITOR temp = m_ObjList.begin(); temp != m_ObjList.end(); temp++)
	{
		(*temp).second->Render();
	}
	return true;
}

bool MSkeletonMgr::Release()
{
	for (ITOR temp = m_ObjList.begin(); temp != m_ObjList.end(); temp++)
	{
		(*temp).second->Release();
		delete (*temp).second;
	}
	return true;
}

MSkeletonMgr::MSkeletonMgr()
{
	iCount = 0;
}


MSkeletonMgr::~MSkeletonMgr()
{
}
