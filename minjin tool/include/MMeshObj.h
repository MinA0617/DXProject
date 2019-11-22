#pragma once
#include "MStd.h"

class MMeshObj
{
public:
	friend class MMeshObjMgr;
public:
	M_STR	m_szName;
	//P3VERTEX *vertices3D;
	//DWORD *index3D;
	vector<P3VERTEX>	VertexData;
	vector<DWORD>		IndexData;
	UINT				m_iVertexCount;
	UINT				m_iIndexCount;
	ID3D11Buffer*		m_pVertexBuffer;
	ID3D11Buffer*		m_pIndexBuffer;
public:
	M_STR				GetName();
	static float3		GetTangetVector(float3 v0, float3 v1, float3 v2, float3 v0_uv, float3 v1_uv, float3 v2_uv, float3 v1_Normal);
	DWORD Check(P3VERTEX data);
	bool CreateBuffer();
	bool Load(M_STR filename);
	bool Release();
public:
	MMeshObj();
	~MMeshObj();
};
