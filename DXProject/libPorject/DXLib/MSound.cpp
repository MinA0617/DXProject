#include "MSound.h"

////정적 변수 초기화
FMOD::System* MSound::m_pSystem = nullptr;
float g_BGMVol = 1.0;
float g_SEVol = 1.0;

MSound::MSound()
{
}

MSound::~MSound()
{
}

bool MSound::Init()
{
	//FMOD::System_Create(&m_pSystem);
	//m_pSystem->init(5, FMOD_INIT_NORMAL, 0);
	return true;
}
bool MSound::Frame()
{

	return true;
}

bool MSound::Render()
{

	return true;
}

bool MSound::Release()
{
	return true;

}

void MSound::Play()
{
	if (m_pSound != nullptr)
	{
		m_pSystem->playSound(m_pSound, 0, false, &m_pChannel);
		m_pChannel->setVolume(g_BGMVol);
		//// BGM볼륨 * 자기 자신의 기본 볼륨으로 설정
		m_pChannel->setMode(FMOD_LOOP_NORMAL);
	}
}


void MSound::PlaySE()
{
	if (m_pSound != nullptr)
	{
		FMOD::Channel* tempChannel;
		m_pSystem->playSound(m_pSound, 0, false, &tempChannel);
		tempChannel->setVolume(g_SEVol);
		tempChannel->setMode(FMOD_LOOP_OFF);
	}
	//if (m_pSound != nullptr)
	//{
	//	m_pSystem->playSound(m_pSound, 0, false, &m_pChannel);
	//	m_pChannel->setVolume(g_BGMVol);
	//	//// BGM볼륨 * 자기 자신의 기본 볼륨으로 설정
	//	m_pChannel->setMode(FMOD_LOOP_OFF);
	//}
}

void MSound::PlayOnec()
{
	bool boo;
	m_pChannel->isPlaying(&boo);
	if (boo == false)
	{
		if (m_pSound != nullptr)
		{
			m_pSystem->playSound(m_pSound, 0, false, &m_pChannel);
			m_pChannel->setVolume(g_BGMVol);
			//// BGM볼륨 * 자기 자신의 기본 볼륨으로 설정
			m_pChannel->setMode(FMOD_LOOP_OFF);
		}
	}
	//if (m_pSound != nullptr)
	//{
	//	m_pSystem->playSound(m_pSound, 0, false, &m_pChannel);
	//	m_pChannel->setVolume(g_BGMVol);
	//	//// BGM볼륨 * 자기 자신의 기본 볼륨으로 설정
	//	m_pChannel->setMode(FMOD_LOOP_OFF);
	//}
}

void MSound::Stop()
{
	int result = m_pChannel->stop();
	m_pChannel->setVolume(0);
}

void MSound::Paused()
{
	bool paused;
	m_pChannel->getPaused(&paused); // 현재상태를 가져옴
	m_pChannel->setPaused(!paused); // 현재상태와 반대로 세트함
}

bool MSound::Load(const char* pSoundFileName)
{
	std::string m_szName = pSoundFileName;//// 멤버변수에 받은 문자열 저장
	bool ret = m_pSystem->createSound(pSoundFileName, FMOD_DEFAULT, 0, &m_pSound); ////사운드 크리에이트
	if (m_pSystem->createSound(pSoundFileName, FMOD_DEFAULT, 0, &m_pSound) != FMOD_OK)
	{
		return false;
	}
}
void MSound::On_Game_Volume(float On_Game_Volume)
{
	//// 1이상은 1로 0이하는 0으로 만든다.
	if (On_Game_Volume > 1)
	{
		On_Game_Volume = 1;
	}
	if (On_Game_Volume < 0)
	{
		On_Game_Volume = 0;
	}
	//// 옵션의 총 조절 볼륨값으로 볼륨이 조절 된다.
	m_pChannel->setVolume(On_Game_Volume*g_BGMVol);
}