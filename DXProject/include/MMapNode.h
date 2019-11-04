#pragma once
#include "MStd.h"

class MMapNode
{
public:
	DWORD					m_dwCurLevel;
	D3DXVECTOR3				m_vWorldPos;
	//DWORD					m_iDepthLevel;
	//float					m_fYMax;
	//bool					m_bIsLeaf;
	//bool					m_IsRender;
	vector<vector<DWORD>>	m_iIndex;
	//DWORD					m_iMiddleIndex[4][6];
	//MMapNode*				m_pNeighborNode[4];
	vector<ID3D11Buffer*>	m_pIndexBuffer;
	//ID3D11Buffer*			m_pMidIndexBuffer[4];

public:
	//bool					CreateMidIndexBuffer();
	bool					CreateIndexBuffer();
	bool					Check(D3DXVECTOR3& camerapos);
	bool					Render();
	bool					Release();
public:
	MMapNode();
	~MMapNode();
};

