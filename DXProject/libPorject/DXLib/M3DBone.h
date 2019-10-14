#pragma once
#include "M3DObject.h"
#include "MAnimateStd.h"
#include "MKeyData.h"
#include "MTimer.h"

// ------------------------------------------------------------------------------------------------------------------------------------------------------------
// Helper Bone Class
// 규칙1. 부모의 스케일을 자식에게 되물림하지 않는다 (디폴트)
// 규칙2. 디폴트는 FK로 IK를 설정하지 않으면 사용할 수 없다.
// 규칙3. FK IK 웨이팅에 대한 0~1의 웨이팅 값을 가지고 있다.
// 규칙4. IK를 적용하지 않으면 pos을 결정할 수 없다.
// 규칙5. IK를 적용한다면 IK상의 부모의 Rotation값은 자식의 pos에 의하여 결정된다
// 규칙6. IK적용시 swivel angle의 회전값(2PI당 순환)을 키값으로 가진다.
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

