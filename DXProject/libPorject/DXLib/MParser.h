#pragma once
#include "MStd.h"
#include "M3DModel.h"
#include "M3DObjectMgr.h"
#include "MKeyAnimationMgr.h"
#include "MUnit.h"
#include "MSkeletonMgr.h"

enum PARSINGTYPE { MESH, SKINNING_MESH, ANIMAITION };

class MParser
{
private:
	vector<string>	m_wordlist;
	typedef vector<string>::iterator ITOR;
	long long		DataToLong(ITOR &data);
	int				DataToInt(ITOR &data);
	float			DataToFloat(ITOR &data);
	MAPTYPE			DataToMAPTYPE(ITOR &data);
	M_STR			DataToMSTR(ITOR &data);
	bool			CreateBuffer(MMesh* Target, vector<MVERTEX>	&vertices, vector<DWORD> &index);
	bool			CreateBuffer(MUnit* unit);
	bool			CreateBuffer(MMesh* Target, vector<DWORD> &index);
	bool			CreateBuffer(M3DBone * Target);
	bool			CheckChar(string::iterator data);
private:
	bool			CreateData(M_STR name);
	bool			CreateGeometryData(ITOR &data, M3DModel* target, MKeyAnimation* ani, M_STR name);
	bool			CreateSkinningData(ITOR &data, MUnit* target);
	bool			CreateBoneData(M_STR name, ITOR &data, M3DBone* target, MKeyAnimation* KeyAni, MSkeleton* skt);
public:
	bool			Load(M_STR filename);
public:
	MParser();
	~MParser();
};

