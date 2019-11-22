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

class MParser : MSingleton<MParser>
{
	friend class MSingleton<MParser>;
protected:
	vector<string>	m_wordlist;
	typedef vector<string>::iterator ITOR;
	long long		DataToLong(ITOR &data);
	int				DataToInt(ITOR &data);
	float			DataToFloat(ITOR &data);
	MAPTYPE			DataToMAPTYPE(ITOR &data);
	//string		DataToSTR(ITOR &data);
	M_STR			DataToMSTR(ITOR &data);
	D3DXVECTOR3			FindMin(vector<MVERTEX>	&vertices);
	D3DXVECTOR3			FindMax(vector<MVERTEX> &vertices);
	bool			CreateBuffer(MMesh* Target, vector<MVERTEX>	&vertices, vector<DWORD> &index);
	bool			CreateBuffer(MMesh* Target, vector<DWORD> &index);
	bool			CheckChar(string::iterator data);
	bool			CreateGeometryData(ITOR &data, M3DModel* target);
	bool			CreateSkinningData(ITOR &data, MSkinModel* target, MSkinMesh* mesh);
	bool			SetBBData(M_STR name, MSkeleton* skt, bool isClear);
	virtual bool	CreateOBJData(M_STR name, MSkeleton* target = nullptr);
	virtual bool	CreateSKTData(M_STR name);
	virtual bool	CreateKEYData(M_STR name);
public:
	bool			Load(M_STR filename, MSkeleton* target = nullptr);
	MFiled*			Load_HM(M_STR filename, float leafsize = 10, float height = 1.0, int minlevel = 64, float lodstartdistance = 1000, bool isChange = true);
	bool			Load_BB(M_STR filename, MSkeleton* skt, bool isClear = true);
private:
	MParser();
public:
	~MParser();
};

#define I_Parser MSingleton<MParser>::GetInstance()