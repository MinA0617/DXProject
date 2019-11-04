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


//// ������۷� ��ġ��, �����̼ǰ��� ������ �־�� �Ѵ�.
//// Ÿ�� ī�޶�� ���� ī�޶��� ������ �ؾ��Ѵ�.
//// ī�޶� ���� ��ư�� �ٲܼ� �־�� �Ѵ�.