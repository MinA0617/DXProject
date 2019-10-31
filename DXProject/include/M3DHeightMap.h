#pragma once
#include "M3DModel.h"
#include "MMeshMgr.h"

class M3DHeightMap : public M3DModel
{
public:
	VertexShader			m_VertexShaderID;
	vector<MVERTEX>			m_VertexList;
	vector<DWORD>			m_IndexList;
	vector<D3DXVECTOR3>		m_FaceNormal;
	vector<vector<int>>		m_VertexFaceIndex;
	int						m_iXCount;
	int						m_iZCount;
	float					m_fYmax;
	float					m_fLeafSize;
	ID3D11Buffer*			m_pVertexBuffer;
	ID3D11Buffer*			m_pIndexBuffer;
public:
	D3DXVECTOR3		ComputeFaceNormal(DWORD i0, DWORD i1, DWORD i2);
	bool			ComputeVertexNormal(DWORD i);
	bool			ComputeNormal();
	bool			CreateBuffer();
	bool			CreateVertex(vector<float>& list);
	bool			Create(M_STR filename, float size);
public:
	virtual bool	Init();
	virtual bool	Frame();
	virtual bool	Render();
	virtual bool	Release();
public:
	M3DHeightMap();
	~M3DHeightMap();
};

