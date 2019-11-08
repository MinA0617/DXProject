#pragma once
#include "MStd.h"
#include "MBoundingBox.h"
enum {UP_, RIGHT_, DOWN_, LEFT_};

class MMapNode
{
public:
	DWORD					m_dwCurLevel;
	D3DXVECTOR3				m_vWorldPos;
	MBoundingBox*			m_Box;
	vector<vector<DWORD>>	m_iIndex;
	vector<vector<DWORD>>	m_iMiddleIndex[4];
	vector<ID3D11Buffer*>	m_pIndexBuffer;
	vector<ID3D11Buffer*>	m_pMidIndexBuffer[4];
	MMapNode*				m_pNeighborNode[4];

public:
	bool					CreateMidIndexBuffer(int XCount);
	bool					CreateIndexBuffer();
	bool					Render();
	bool					Release();
public:
	MMapNode();
	~MMapNode();
};

