#pragma once
#include "MParser.h"
class MBBParser : public MParser
{
public:
	bool CreateData(M_STR name, bool isGeo, bool isBone, MSkeleton* skt2) override;
public:
	MBBParser();
	~MBBParser();
};

