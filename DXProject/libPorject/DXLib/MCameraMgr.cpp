#include "MCameraMgr.h"



MCameraMgr::MCameraMgr()
{
	m_MainCamera = NULL;
}


MCameraMgr::~MCameraMgr()
{
}

bool MCameraMgr::Init()
{
	m_MainCamera = new MCamera;
	m_MainCamera->Init();
	return true;
}

bool MCameraMgr::Release()
{
	SAFE_RELEASE(m_MainCamera);
	return true;
}

bool MCameraMgr::Frame()
{
	m_MainCamera->Frame();
	frustum.CreateFrustum(m_MainCamera->m_matView, m_MainCamera->m_matProj);
	return true;
}

bool MCameraMgr::Render()
{
	m_MainCamera->Render();
	m_pGrobalCameraBuffer = m_MainCamera->m_pConstantBuffer;
	return true;
}

bool MCameraMgr::CreateFreeCamera_Main()
{
	SAFE_RELEASE(m_MainCamera);
	delete m_MainCamera;
	m_MainCamera = new MFreeCamera;
	m_MainCamera->Init();
	return true;
}

bool MCameraMgr::CreateFPSCamera_Main()
{
	SAFE_RELEASE(m_MainCamera);
	delete m_MainCamera;
	m_MainCamera = new MFPSCamera;
	m_MainCamera->Init();
	return true;
}

bool MCameraMgr::CreateTargetCamera_Main(M3DObject* target)
{
	SAFE_RELEASE(m_MainCamera);
	delete m_MainCamera;
	MTargetCamera* temp = new MTargetCamera;
	temp->SetTarget(target);
	m_MainCamera = temp;
	m_MainCamera->Init();
	return true;
}

bool MCameraMgr::CreateRPGCamera_Main(M3DObject* target)
{
	SAFE_RELEASE(m_MainCamera);
	delete m_MainCamera;
	MRPGCamera* temp = new MRPGCamera;
	temp->SetTarget(target);
	m_MainCamera = temp;
	m_MainCamera->Init();
	return true;
}