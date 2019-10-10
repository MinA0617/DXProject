#pragma once
#include "MKeyData.h"

class MKeyAnimation
{
public:
	typedef std::map<M_STR, MKeyData*>::iterator ITOR;
	map<M_STR, MKeyData*>			m_List;
public:

	bool							Init();
	bool							Release();
public:	
	MKeyAnimation();
	~MKeyAnimation();
};

