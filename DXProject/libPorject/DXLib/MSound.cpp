#include "MSound.h"

////���� ���� �ʱ�ȭ
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
		//// BGM���� * �ڱ� �ڽ��� �⺻ �������� ����
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
	//	//// BGM���� * �ڱ� �ڽ��� �⺻ �������� ����
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
			//// BGM���� * �ڱ� �ڽ��� �⺻ �������� ����
			m_pChannel->setMode(FMOD_LOOP_OFF);
		}
	}
	//if (m_pSound != nullptr)
	//{
	//	m_pSystem->playSound(m_pSound, 0, false, &m_pChannel);
	//	m_pChannel->setVolume(g_BGMVol);
	//	//// BGM���� * �ڱ� �ڽ��� �⺻ �������� ����
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
	m_pChannel->getPaused(&paused); // ������¸� ������
	m_pChannel->setPaused(!paused); // ������¿� �ݴ�� ��Ʈ��
}

bool MSound::Load(const char* pSoundFileName)
{
	std::string m_szName = pSoundFileName;//// ��������� ���� ���ڿ� ����
	bool ret = m_pSystem->createSound(pSoundFileName, FMOD_DEFAULT, 0, &m_pSound); ////���� ũ������Ʈ
	if (m_pSystem->createSound(pSoundFileName, FMOD_DEFAULT, 0, &m_pSound) != FMOD_OK)
	{
		return false;
	}
}
void MSound::On_Game_Volume(float On_Game_Volume)
{
	//// 1�̻��� 1�� 0���ϴ� 0���� �����.
	if (On_Game_Volume > 1)
	{
		On_Game_Volume = 1;
	}
	if (On_Game_Volume < 0)
	{
		On_Game_Volume = 0;
	}
	//// �ɼ��� �� ���� ���������� ������ ���� �ȴ�.
	m_pChannel->setVolume(On_Game_Volume*g_BGMVol);
}