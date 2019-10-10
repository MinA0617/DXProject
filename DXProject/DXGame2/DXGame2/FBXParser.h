#pragma once
#include "MStd.h"
#include "MKeyAnimationMgr.h"

//Geometry: 3029714928944 오브젝트의 ID
//Vertices: 3개씩 물체의 xyz성분
//PolygonVertexIndex: ?? 이게 뭐지 시발
//Normals: 버텍스1번부터 노말

enum FBXTYPE { MESH, SKINNING_MESH, ANIMAITION };

struct int3
{
	int v1, v2, v3;
	int3()
	{}
	int3(int iv1, int iv2, int iv3)
	{
		v1 = iv1;
		v2 = iv2;
		v3 = iv3;
	}
};

class FBXParser
{
private:
	vector<string>	m_wordlist;
	typedef vector<string>::iterator ITOR;
private:
	long long		DataToLong(ITOR &data);
	int				DataToInt(ITOR &data);
	float			DataToFloat(ITOR &data);
	bool			CheckChar(string::iterator data);
private:
	bool			CreateMeshData(M_STR name);
	bool			CreateSkinnigMeshData(M_STR name);
	bool			CreateAnimationData(M_STR name);
public:
	bool			Load(M_STR filename, FBXTYPE type);
public:
	FBXParser();
	~FBXParser();
};

