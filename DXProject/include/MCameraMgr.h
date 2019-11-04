#pragma once
#include "MFreeCamera.h"
#include "MFPSCamera.h"
#include "MRPGCamera.h"
#include "MFrustum.h"

class MCameraMgr : public MSingleton<MCameraMgr>
{
	friend class MSingleton<MCameraMgr>;
public:
	MCamera* m_MainCamera;
	MFrustum frustum;
public:
	bool CreateFreeCamera_Main();
	bool CreateFPSCamera_Main();
	bool CreateTargetCamera_Main(M3DObject* target);
	bool CreateRPGCamera_Main(M3DObject* target);
public:
	ID3D11Buffer* m_pGrobalCameraBuffer;
public:
	virtual bool Init();
	virtual bool Frame();
	virtual bool Render();
	virtual bool Release();
private:
	MCameraMgr();
public:
	~MCameraMgr();
};

#define I_CameraMgr MCameraMgr::GetInstance()

//// 카메라매니저
//// main카메라를 포인터로
//// 서브카메라는 벡터로 (한참 나중에 추가)
//// Init에서 디폴트 카메라 생성
//// 함수로 다른 카메라 생성시엔 메인카메라를 릴리즈하고 추가
//// 서브카메라는