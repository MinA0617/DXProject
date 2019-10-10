#pragma once
#include "MStd.h"



class MTimer : public MSingleton<MTimer>
{
private:
	LARGE_INTEGER	m_StartTime;				// 타이머 실행 시간
	LARGE_INTEGER	m_Frequency;				// 초당 주파수
	LARGE_INTEGER	m_CurrentFrameTime;			// 현재시간
	LARGE_INTEGER	m_BeforeFrameTime;			// 이전 프레임의 시간체크
	LARGE_INTEGER	m_FramePerSecond;			// 1초 당 프레임
	int				m_iFramePerSecond;			// 1초 당 프레임(int) / 보관용
	int				m_iFramePerSecondElapse;	// 1초 당 프레임 / 세는용
	float			m_fSecondPerFrame;			// 1프레임 당 초(시간)
	int				m_itime;						// 경과시간 (초)
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
