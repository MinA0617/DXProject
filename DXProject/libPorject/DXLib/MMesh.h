#pragma once
#include "MStd.h"
#include "VertexShaderMgr.h"

class MSkeleton;
class MMesh
{
public:
	friend class MMeshMgr;
public:
	VertexShader		m_VertexShaderID;
public:
	//vector<MVERTEX>		VertexData;
	//vector<DWORD>		IndexData;
	//UINT				m_iVertexCount;
	UINT				m_iIndexCount;
	ID3D11Buffer*		m_pVertexBuffer;
	ID3D11Buffer*		m_pIndexBuffer;
public:
	bool				SetSkeletonAndCreateData(MSkeleton* skt, vector<CVERTEX>& OldData) { return false; };
public:
	virtual bool Init();
	virtual bool Frame();
	virtual bool Render();
	virtual bool Release();
public:
	MMesh();
	virtual ~MMesh();
};
