#pragma once
#include "M3DObject.h"
#include "MAnimateStd.h"
#include "MKeyData.h"
#include "MTimer.h"

// ------------------------------------------------------------------------------------------------------------------------------------------------------------
// Helper Bone Class
// ��Ģ1. �θ��� �������� �ڽĿ��� �ǹ������� �ʴ´� (����Ʈ)
// ��Ģ2. ����Ʈ�� FK�� IK�� �������� ������ ����� �� ����.
// ��Ģ3. FK IK �����ÿ� ���� 0~1�� ������ ���� ������ �ִ�.
// ��Ģ4. IK�� �������� ������ pos�� ������ �� ����.
// ��Ģ5. IK�� �����Ѵٸ� IK���� �θ��� Rotation���� �ڽ��� pos�� ���Ͽ� �����ȴ�
// ��Ģ6. IK����� swivel angle�� ȸ����(2PI�� ��ȯ)�� Ű������ ������.
// ------------------------------------------------------------------------------------------------------------------------------------------------------------
class M3DBone : public M3DObject
{
public:
	virtual void		SetLocalPosition(D3DXVECTOR3 data) override;
	virtual void		SetLocalRotation(D3DXQUATERNION data);
	virtual void		SetLocalScale(D3DXVECTOR3 data) override;
	virtual void		ModifyLocalPosition(D3DXVECTOR3 data) override;
	virtual void		ModifyLocalRotation(D3DXQUATERNION data);
	virtual void		ModifyLocalScale(D3DXVECTOR3 data) override;
	D3DXMATRIX			ZeroMatrix;
	MKeyData*			m_KeyData;
	MKeyData			m_TempKeyData;
public:
	virtual bool		UpdateKey(float time);
	virtual	bool		SetZero();
	virtual bool		Init() override;
	virtual bool		Frame() override;
	virtual bool		Render() override;
public:
	ID3D11Buffer*		m_pVertexBuffer;
	ID3D11Buffer*		m_pIndexBuffer;
	DWORD				m_iVertexCount;
	DWORD				m_iIndexCount;
public:
	M3DBone();
	~M3DBone();

};

