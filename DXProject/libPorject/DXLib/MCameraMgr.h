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

//// ī�޶�Ŵ���
//// mainī�޶� �����ͷ�
//// ����ī�޶�� ���ͷ� (���� ���߿� �߰�)
//// Init���� ����Ʈ ī�޶� ����
//// �Լ��� �ٸ� ī�޶� �����ÿ� ����ī�޶� �������ϰ� �߰�
//// ����ī�޶��