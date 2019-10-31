#pragma once
#include "MStd.h"
#include "M3DModel.h"
#include "M3DObjectMgr.h"
#include "MKeyAnimationMgr.h"
#include "MSkinModel.h"
#include "MSkinMesh.h"
#include "MSkeletonMgr.h"
#include "M3DHeightMap.h"

// -------------------------------------------------------------------------------------
// ���� ������Ʈ
// ��Ų ������Ʈ
// �Ϲ� ������Ʈ
// �ε�� ���ÿ� Mgr�� �߰��Ͽ� �ѷ���
// 3D������Ʈ �Ŵ������� ��ũ�� �
// -------------------------------------------------------------------------------------
enum PARSINGTYPE { MESH, SKINNING_MESH, ANIMAITION };

class MParser
{
protected:
	vector<string>	m_wordlist;
	typedef vector<string>::iterator ITOR;
	long long		DataToLong(ITOR &data);
	int				DataToInt(ITOR &data);
	float			DataToFloat(ITOR &data);
	MAPTYPE			DataToMAPTYPE(ITOR &data);
	//string		DataToSTR(ITOR &data);
	M_STR			DataToMSTR(ITOR &data);
	float3			FindMin(vector<MVERTEX>	&vertices);
	float3			FindMax(vector<MVERTEX> &vertices);
	bool			CreateBuffer(MMesh* Target, vector<MVERTEX>	&vertices, vector<DWORD> &index);
	//bool			CreateBuffer(MSkinModel* unit);
	bool			CreateBuffer(MMesh* Target, vector<DWORD> &index);
	bool			CheckChar(string::iterator data);
	virtual bool	CreateData(M_STR name, bool isGeo, bool isBone, MSkeleton* skt);
	bool			CreateGeometryData(ITOR &data, M3DModel* target, MKeyAnimation* ani, M_STR name);
	bool			CreateSkinningData(ITOR &data, MSkinModel* target, MSkinMesh* mesh);
	bool			CreateBoneData(M_STR name, ITOR &data, M3DBone* target, MKeyAnimation* KeyAni, MSkeleton* skt);
	bool			SetBBData(M_STR name, MUnit* unit, bool isClear);
public:
	bool			Load(M_STR filename, bool isGeo = true, bool isBone = true, MSkeleton* skt = nullptr);
	bool			Load_HM(M_STR filename, float size);
	bool			Load_BB(M_STR filename, MUnit* unit, bool isClear = true);
public:
	MParser();
	~MParser();
};

