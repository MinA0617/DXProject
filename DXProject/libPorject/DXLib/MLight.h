#pragma once
#include "M3DObject.h"

// ----------------------------------------------------------------------------------------
// Light Class
// - 이 클래스에서는 조명 오브젝트를 다룹니다.
// - 순수 가상 클래스입니다.
// ----------------------------------------------------------------------------------------

enum LightType {SPOT, SKY, DIRECT, OMNI,};

struct CONSTANT_LIGHT
{
	D3DXMATRIX		m_matInvWorld;
	D3DXVECTOR3		m_Pos;				// 월드 좌표
	float			m_fDistance;		// 거리
	D3DXVECTOR3		m_Direction;		// 방향
	float			m_fMultiplier;		// 빛의 세기
	D3DXVECTOR3		m_Color;			// 빛의 컬러값
	float			m_fInner;			// 스팟의 중심에서의 감쇠시작거리
	float			m_fOutner;			// 스팟의 각도
	float			m_fOffset;			// 라이트와 물체 사이 거리 감쇠
	LightType		m_iFlag;			// 속성 플래그 값
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

