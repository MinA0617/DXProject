#pragma once
#include "M3DModel.h"
#include "MMeshMgr.h"
#include "MMapNode.h"
#include "MCameraMgr.h"


class M3DHeightMap : public M3DModel
{
public:

	vector<MMapNode*>		m_List;
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
	bool					m_bIsLOD;
	int						m_iMaxLevel;
	float					m_fLodStartDistance;
	int						m_iLodMinLevel;
public:
	int						m_iIndexingIndex;
	D3DXVECTOR3				ComputeFaceNormal(DWORD i0, DWORD i1, DWORD i2);
	bool					SetLevelOfDetail(float startdistancs, int MinLevel);
	bool					ComputeVertexNormal(DWORD i);
	bool					ComputeNormal();
	bool					IndexingNode(MMapNode* node, int level, int XSize, DWORD flag);
	void					CheckNode(MMapNode* node);
	bool					CreateNode();
	bool					CreateBuffer();
	bool					CreateVertex(vector<float>& list);
	bool					Create(M_STR filename, float size, bool lod, int minlevel);
public:
	virtual bool			Init();
	virtual bool			Frame();
	virtual bool			Render();
	virtual bool			Release();
public:
	M3DHeightMap();
	~M3DHeightMap();
};

