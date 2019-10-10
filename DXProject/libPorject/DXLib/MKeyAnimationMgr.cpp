#include "MKeyAnimationMgr.h"



MKeyAnimationMgr::MKeyAnimationMgr()
{
}


MKeyAnimationMgr::~MKeyAnimationMgr()
{
}


MKeyAnimation* MKeyAnimationMgr::NewKeyAnimation(M_STR name)
{
	//M3DBone* newbone = new M3DBone
	ITOR findnode = m_KeyAniList.find(name);
	if (findnode == m_KeyAniList.end());
	{
		MKeyAnimation* data = new MKeyAnimation;
		m_KeyAniList.insert(std::make_pair(name, data));
		return data;
	}
	return (*findnode).second;
}

bool MKeyAnimationMgr::Release()
{
	for (ITOR data = m_KeyAniList.begin(); data != m_KeyAniList.end(); data++)
	{
		(*data).second->Release();
	}
	return true;
}