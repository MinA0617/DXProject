#pragma once
#include "M3DObject.h"

// ----------------------------------------------------------------------------------------
// Light Class
// - �� Ŭ���������� ���� ������Ʈ�� �ٷ�ϴ�.
// - ���� ���� Ŭ�����Դϴ�.
// ----------------------------------------------------------------------------------------

enum LightType {SPOT, SKY, DIRECT, OMNI,};

struct CONSTANT_LIGHT
{
	D3DXMATRIX		m_matInvWorld;
	D3DXVECTOR3		m_Pos;				// ���� ��ǥ
	float			m_fDistance;		// �Ÿ�
	D3DXVECTOR3		m_Direction;		// ����
	float			m_fMultiplier;		// ���� ����
	D3DXVECTOR3		m_Color;			// ���� �÷���
	float			m_fInner;			// ������ �߽ɿ����� ������۰Ÿ�
	float			m_fOutner;			// ������ ����
	float			m_fOffset;			// ����Ʈ�� ��ü ���� �Ÿ� ����
	LightType		m_iFlag;			// �Ӽ� �÷��� ��
	float			temp;
};

class MLight : public M3DObject
{
public:
	CONSTANT_LIGHT	m_ConstantLigth;
	ID3D11Buffer*	m_pLightConstantBuffer;
public:
	virtual bool	CreateConstantBuffer() override;
	virtual bool	Init() override;
	virtual bool	Frame() override;
	virtual bool	Render() override;
public:
	MLight();
	~MLight();
};

