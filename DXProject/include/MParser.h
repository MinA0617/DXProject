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
// 헬퍼 오브젝트
// 스킨 오브젝트
// 일반 오브젝트
// 로드와 동시에 Mgr에 추가하여 뿌려짐
// 3D오브젝트 매니저에서 뱅크도 운영
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
	// 인스턴스 용
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