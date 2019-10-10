#include "MTimer.h"

float g_fSeoundPerFrame = 0.0f;
float g_iFramePerSecond = 0.0f;

MTimer::MTimer()
{
}


MTimer::~MTimer()
{
}

bool MTimer::Init()
{
	QueryPerformanceFrequency((LARGE_INTEGER*)&m_Frequency);
	//// 실행 가능 여부 판단 ////
	QueryPerformanceCounter(&m_StartTime);
	if (m_StartTime.QuadPart == 0) return false;
	m_FramePerSecond = m_StartTime;
	m_BeforeFrameTime = m_StartTime;
	m_iFramePerSecond = 0;
	m_iFramePerSecondElapse = 0;
	m_fSecondPerFrame = 0;
	m_itime = 0;
	return true;
}

bool MTimer::Frame()
{
	QueryPerformanceCounter(&m_CurrentFrameTime); // 현재시간 가져오기
	m_fSecondPerFrame = static_cast<float>(m_CurrentFrameTime.QuadPart - m_BeforeFrameTime.QuadPart) / static_cast<float>(m_Frequency.QuadPart);
	g_fSeoundPerFrame = m_fSecondPerFrame; // SPF 전역화

	if (((m_CurrentFrameTime.LowPart - m_FramePerSecond.LowPart) / m_Frequency.LowPart) >= 1)
	{
		m_iFramePerSecond = m_iFramePerSecondElapse;
		m_iFramePerSecondElapse = 0;
		m_FramePerSecond = m_CurrentFrameTime;
		m_itime++;
	}
	else
	{
		m_iFramePerSecondElapse++;
	}
	m_BeforeFrameTime = m_CurrentFrameTime;
	return true;
}

bool MTimer::Render()
{
	return true;
}

bool MTimer::Release()
{
	return true;
}

LARGE_INTEGER MTimer::GetStartTimeM()
{
	return m_StartTime;
}

LARGE_INTEGER MTimer::GetCurrentTimeM()
{
	return m_CurrentFrameTime;
}

int	MTimer::GetFramePerSecond()
{
	return m_iFramePerSecond;
}