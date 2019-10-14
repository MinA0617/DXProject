#pragma once
#include "MStd.h"

class MMesh
{
public:
	friend class MMeshMgr;
public:
	vector<MVERTEX>		VertexData;
	vector<DWORD>		IndexData;
	UINT				m_iVertexCount;
	UINT				m_iIndexCount;
	ID3D11Buffer*		m_pVertexBuffer;
	ID3D11Buffer*		m_pIndexBuffer;
public:
	bool Release();
public:
	MMesh();
	~MMesh();
};
