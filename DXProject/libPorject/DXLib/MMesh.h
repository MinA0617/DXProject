#pragma once
#include "MStd.h"

class MMesh
{
public:
	friend class MMeshMgr;
public:
	M_STR	m_szName;
	//P3VERTEX *vertices3D;
	//DWORD *index3D;
	vector<MVERTEX>		VertexData;
	vector<DWORD>		IndexData;
	UINT				m_iVertexCount;
	UINT				m_iIndexCount;
	ID3D11Buffer*		m_pVertexBuffer;
	ID3D11Buffer*		m_pIndexBuffer;
public:
	M_STR				GetName();
	//static float3		GetTangetVector(float3 v0, float3 v1, float3 v2, float3 v0_uv, float3 v1_uv, float3 v2_uv, float3 v1_Normal);
	//DWORD Check(MVERTEX data);
	//bool CreateBuffer();
	//bool Load(M_STR filename);
	bool Release();
public:
	MMesh();
	~MMesh();
};
