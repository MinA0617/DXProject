#pragma once
#include "MStd.h"



class MTimer : public MSingleton<MTimer>
{
private:
	LARGE_INTEGER	m_StartTime;				// Ÿ�̸� ���� �ð�
	LARGE_INTEGER	m_Frequency;				// �ʴ� ���ļ�
	LARGE_INTEGER	m_CurrentFrameTime;			// ����ð�
	LARGE_INTEGER	m_BeforeFrameTime;			// ���� �������� �ð�üũ
	LARGE_INTEGER	m_FramePerSecond;			// 1�� �� ������
	int				m_iFramePerSecond;			// 1�� �� ������(int) / ������
	int				m_iFramePerSecondElapse;	// 1�� �� ������ / ���¿�
	float			m_fSecondPerFrame;			// 1������ �� ��(�ð�)
	int				m_itime;						// ����ð� (��)
public:
	LARGE_INTEGER	GetStartTimeM();
	LARGE_INTEGER	GetCurrentTimeM();
	int				GetFramePerSecond();
public:
	bool Init();
	bool Frame();
	bool Render();
	bool Release();
public:
	MTimer();
	virtual ~MTimer();
};

#define I_Timer MTimer::GetInstance()
