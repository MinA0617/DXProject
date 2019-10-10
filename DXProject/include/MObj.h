#pragma once
#include "MStd.h"

class MObj
{
public:
	friend class MObjMgr;
public:
	M_STR	m_szName;
	P3VERTEX *vertices3D;
	DWORD *index3D;
//private:
	ID3D11Buffer*		m_pVertexBuffer;
	ID3D11Buffer*		m_pIndexBuffer;
	DWORD				m_iVertexCount;
	DWORD				m_iIndexCount;
public:
	M_STR				GetName();
	bool CreateBuffer();
	bool Load(M_STR filename);
	bool Release();
public:
	MObj();
	~MObj();
};
