#pragma once
#include "MStd.h"
#include "fmod.hpp"

class MSound
{
public:
	MSound();
	virtual	~MSound();
	bool Init();
	bool Frame();
	bool Render();
	bool Release();
public:
	static FMOD::System* m_pSystem;
	int iIndex;
public:
	M_STR m_szName;
public:
	FMOD::Sound* m_pSound;
	FMOD::Channel* m_pChannel;
	bool Load(const char* pSoundFileName);
	void Play();
	void PlaySE();
	void PlayOnec();
	void Stop();
	void Paused();
	void On_Game_Volume(float On_Game_Volume);
};