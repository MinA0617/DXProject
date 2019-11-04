#pragma once
#include "M3DObject.h"
#define MAXDISTANCE 10000.0f
class MCamera : public M3DObject
{
	friend class MCameraMgr;
public:
	D3DXMATRIX			m_matWorld;
	D3DXMATRIX			m_matView;
	D3DXMATRIX			m_matProj;
	float				yew;
	float				pitch;
	D3DXVECTOR3			m_LookAt;
	D3DXVECTOR3			m_Up;
public:
	BOOL				isRotition;
public:
	bool				CreateConstantBuffer() override;
	ID3D11Buffer*		m_pConstantBuffer;
public:
	virtual void		MoveFront(float valve);
	virtual void		MoveBack(float valve);
	virtual void		MoveRight(float valve);
	virtual void		MoveLeft(float valve);

	virtual bool		Init() override;
	virtual bool		Frame() override;
	virtual bool		Render() override;
	virtual bool		Release() override;
public:
	MCamera();
	~MCamera();

};


//// 상수버퍼로 위치값, 로테이션값을 가지고 있어야 한다.
//// 타켓 카메라와 프리 카메라의 구분을 해야한다.
//// 카메라 조작 버튼을 바꿀수 있어야 한다.