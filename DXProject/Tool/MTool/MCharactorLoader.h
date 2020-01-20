#pragma once
#include "MStd.h"
#include "MParser.h"

class MCharactorLoader
{
public:
	M_STR	AnotherName(M_STR oldpath, M_STR ext);
	M_STR	SKTLoad(M_STR path);
	bool	BBLoad(M_STR path, M_STR sktname);
	bool	OBJLoad(M_STR path, M_STR sktname);
	bool	KEYLoad(M_STR path);
	bool	CreatUnit(M_STR sktname, M_STR* path, int count);
public:
	bool	Load(M_STR path);
	MCharactorLoader();
	~MCharactorLoader();
};

