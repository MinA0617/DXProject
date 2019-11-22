#pragma once
#include "MStd.h"
#include "MMaterial.h"
#include "MBoundingBox.h"

#define MAX_MAP_COUNT 2

enum {UP_, RIGHT_, DOWN_, LEFT_};

struct CONSTANT_MAPNODE
{
	D3DXVECTOR3	m_ObjectColor;
	int			m_isAlpha;
	D3DXVECTOR2	m_WorldSize;
	int			m_isDiffuse[2];
	int			m_isNormal[2];
	int			temp0;
	int			temp1;
};

class MMapNode
{
public:
	CONSTANT_MAPNODE		m_ConstantMap;
	DWORD					m_dwCurLevel;
	D3DXVECTOR3				m_vWorldPos;
	MBoundingBox*			m_Box;
	vector<vector<DWORD>>	m_iIndex;
	vector<vector<DWORD>>	m_iMiddleIndex[4];
	vector<ID3D11Buffer*>	m_pIndexBuffer;
	vector<ID3D11Buffer*>	m_pMidIndexBuffer[4];
	MMapNode*				m_pNeighborNode[4];
public:
	ID3D11Buffer*			m_pConstantBuffer;
	int						m_pDiffuseMap[MAX_MAP_COUNT];
	int						m_pNormalMap[MAX_MAP_COUNT];
public:
	bool					UpdateConstantBuffer();
	bool					SetTexture(DWORD tex, MAPTYPE type, UINT layer = 0);
	bool					CreateMidIndexBuffer(int XCount);
	bool					CreateIndexBuffer();
	bool					Render();
	bool					Release();
public:
	MMapNode();
	~MMapNode();
};

