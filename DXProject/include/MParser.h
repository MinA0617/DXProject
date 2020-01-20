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
	D3DXVECTOR3		FindMin(vector<MVERTEX>	&vertices);
	D3DXVECTOR3		FindMax(vector<MVERTEX> &vertices);
	M_STR			AnotherPath(M_STR oldpath);
	bool			Load_Map(M3DInstanceModel* target, M_STR name, MAPTYPE maptype);
	bool			CreateVertexBuffer(MMesh* Target, vector<MVERTEX>	&vertices);
	bool			CreateIndexBuffer(MMesh* Target, vector<DWORD> &index);
	bool			CheckChar(string::iterator data);
	//bool			CreateGeometryData(ITOR &data, M3DModel* target);
	//bool			CreateSkinningData(ITOR &data, MSkinModel* target, MSkinMesh* mesh);
	bool			SetBBData(M_STR name, MSkeleton* skt, bool isClear);
	//bool			CreateOBJData(M_STR name, MSkeleton* target = nullptr);
	bool			CreateSKTData(M_STR name);
	bool			CreateKEYData(M_STR name);
public:
	// -------------------------------------------------------------------------------------------
	// �ν��Ͻ� ��
	bool			CreateSkinMeshData(ITOR &data, M3DInstanceModel* target, MSkeleton* skt);
	bool			CreateMeshData(ITOR &data, M3DInstanceModel* target);
	bool			CreateInstanceOBJData(vector<string>& list, vector<M_STR>& namelist, MSkeleton* target);
	bool			ParsingDate(M_STR filename, vector<string>& list);
	bool			Load(M_STR filename, vector<M_STR>& namelist, MSkeleton* target = nullptr);
	bool			Load(M_STR filename);
	MFiled*			Load_HM(M_STR filename, float leafsize = 10, float height = 1.0, int minlevel = 64, float lodstartdistance = 1000, bool isChange = true);
	bool			Load_BB(M_STR filename, MSkeleton* skt, bool isClear = true);
private:
	MParser();
public:
	~MParser();
};

#define I_Parser MSingleton<MParser>::GetInstance()