#pragma once
#include "M3DModel.h"
#include "MMeshMgr.h"
#include "MMapNode.h"
#include "MCameraMgr.h"

class M3DHeightMap : public M3DModel
{
public:
	vector<MMapNode*>			m_List;					// 타일 리스트
	vector<MVERTEX>				m_VertexList;			// 버텍스 데이터
	vector<DWORD>				m_IndexList;			// 인덱스 데이터
	vector<D3DXVECTOR3>			m_FaceNormal;			// 페이스의 노말
	vector<vector<int>>			m_VertexFaceIndex;		// 버텍스가 포함된 페이스 목록
	//vector<DWORD[3]>			m_FaceVertexIndex;		// 페이스가 팜조하는 버텍스 3개
public:
	VertexShader				m_VertexShaderID;
	ID3D11Texture2D*			m_pHeightTexture;

	vector<DWORD>				m_TextureList;
	ID3D11ShaderResourceView*	m_pAlphaTexture;

	ID3D11Buffer*				m_pVertexBuffer;
	ID3D11Buffer*				m_pIndexBuffer;
	int							m_iCount;
	float						m_fYmax;
	float						m_fLeafSize;
	int							m_iMaxLevel;
	float						m_fLodStartDistance;
	int							m_iLodMinLevel;
	int							m_iTileSize;
	int							m_iIndexingIndex;
public:
	bool						SetTexture(MMapNode* node, UINT textid, MAPTYPE type, UINT layerid);
public:
	float						FindLeafYMax(float i0x, float i0z);
	D3DXVECTOR3					ComputeFaceNormal(DWORD i0, DWORD i1, DWORD i2);
	bool						ComputeVertexNormal(DWORD i);
	bool						ComputeFaceNormalAndFaceIndexing();
	bool						IndexingNode(MMapNode* node, int level, int XSize, DWORD flag);
	void						CheckNode(MMapNode* node);
	bool						CreateNode();
	void						SetNeighborNode();
	bool						CreateBuffer();
	bool						CreateVertex();
	bool						CreateVertex(vector<float>& list);
	bool						Check();
	bool						Load_MAP(M_STR filename, MAPTYPE type = NOTFOUND);
	bool						CreateAlphaTexture(int xsize, int ysize = -1);
	bool						Create(M_STR filename, float leafsize, float height, int tilesiza, float lodstartdistance);
	bool						Create(M_STR name, int count, float size, int tilesiza, float lodstartdistance);
public:
	virtual bool				Init();
	virtual bool				Frame();
	virtual bool				Render();
	virtual bool				Release();
public:
	M3DHeightMap();
	~M3DHeightMap();
};

