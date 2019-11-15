#pragma once
#include "M3DModel.h"
#include "MMeshMgr.h"
#include "MMapNode.h"
#include "MCameraMgr.h"

class M3DHeightMap : public M3DModel
{
public:
	vector<MMapNode*>			m_List;					// Ÿ�� ����Ʈ
	vector<MVERTEX>				m_VertexList;			// ���ؽ� ������
	vector<DWORD>				m_IndexList;			// �ε��� ������
	vector<D3DXVECTOR3>			m_FaceNormal;			// ���̽��� �븻
	vector<vector<int>>			m_VertexFaceIndex;		// ���ؽ��� ���Ե� ���̽� ���
	//vector<DWORD[3]>			m_FaceVertexIndex;		// ���̽��� �����ϴ� ���ؽ� 3��
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

