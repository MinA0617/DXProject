#include "MGrid.h"



MGrid::MGrid()
{
}


MGrid::~MGrid()
{
}

bool MGrid::Init()
{
	I_MaterialMgr[MaterialID]->m_PixelShaderID = PSSPLINE;

	SetGrid(1000, 50);
	return true;
}

bool MGrid::SetGrid(UINT GridSize, UINT StepSize)
{
	//// �׸��� ������� 4��и��� ������ ����� �̾߱���
	m_GridSize = GridSize;
	m_StepSize = StepSize;
	if (GridSize / StepSize < 1)
	{
		return false;
	}
	///// ���� ������ ���� /////
	SAFE_RELEASE(m_pVertexBuffer);
	SAFE_RELEASE(m_pIndexBuffer);


#pragma region CreateData
	m_iVertexCount = ((GridSize / StepSize) * 8) + 4;
	m_iIndexCount = m_iVertexCount;
	vector<SPLINE_VERTEX> vertices;
	vector<DWORD> index;
	vertices.resize(m_iVertexCount);
	index.resize(m_iIndexCount);

	//// ��ġ����
	int iSize = (GridSize / StepSize) * StepSize;
	//// ù XY�߱� 
	vertices[0].p = float3(iSize, 0, 0);
	vertices[1].p = float3(-iSize, 0, 0);
	vertices[2].p = float3(0, 0, iSize);
	vertices[3].p = float3(0, 0, -iSize);
	for (int i = 1; i <= (m_iVertexCount - 4) / 8; i++)
	{
		int CurStepSize = i * StepSize;
		vertices[4 + ((i - 1) * 8) + 0].p = float3(iSize, 0, CurStepSize);
		vertices[4 + ((i - 1) * 8) + 1].p = float3(-iSize, 0, CurStepSize);
		vertices[4 + ((i - 1) * 8) + 2].p = float3(iSize, 0, -CurStepSize);
		vertices[4 + ((i - 1) * 8) + 3].p = float3(-iSize, 0, -CurStepSize);
		vertices[4 + ((i - 1) * 8) + 4].p = float3(CurStepSize, 0, iSize);
		vertices[4 + ((i - 1) * 8) + 5].p = float3(CurStepSize, 0, -iSize);
		vertices[4 + ((i - 1) * 8) + 6].p = float3(-CurStepSize, 0, iSize);
		vertices[4 + ((i - 1) * 8) + 7].p = float3(-CurStepSize, 0, -iSize);
	}
	for (int i = 0; i < m_iIndexCount; i++)
	{
		//vertices[i * 2].n = float3(0, 1, 0);
		//vertices[i * 2].type = NULL;
		//vertices[i * 2 + 1].n = float3(0, 1, 0);
		//vertices[i * 2 + 1].type = NULL;
		vertices[i].n = float3(0, 1, 0);
		vertices[i].type = NULL;
		index[i] = i;
	}
#pragma endregion CreateData


#pragma region CreateBuffer
	D3D11_BUFFER_DESC BufferDesc;				//// ���� ���� ����ũ
	D3D11_SUBRESOURCE_DATA SubresourceData;		//// ���� ���ҽ� ������
	HRESULT CreateBufferResult;					//// ���� �����
		///// ���ؽ� ���� ///////////////////////////////////////////////////////////////////////////////////////////////////
	ZeroMemory(&BufferDesc, sizeof(D3D11_BUFFER_DESC));
	BufferDesc.ByteWidth = sizeof(SPLINE_VERTEX) * m_iVertexCount;	//// ������ (Byte����)
	BufferDesc.Usage = D3D11_USAGE_DEFAULT;							//// ����� (DEFAULT�� �׷���ī�忡 ����)
	BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;				//// � ���۷� ���Ǵ���
	ZeroMemory(&SubresourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	SubresourceData.pSysMem = &vertices.at(0);
	if (FAILED(CreateBufferResult = g_pDevice->CreateBuffer(&BufferDesc, &SubresourceData, &m_pVertexBuffer))) return false;
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//// �ε��� ���� /////////////////////////////////////////////////////////////////////////////////////////////////////
	ZeroMemory(&BufferDesc, sizeof(D3D11_BUFFER_DESC));
	BufferDesc.ByteWidth = sizeof(DWORD) * m_iIndexCount;	//// ������ (Byte����)
	BufferDesc.Usage = D3D11_USAGE_DEFAULT;							//// ����� (DEFAULT�� �׷���ī�忡 ����)
	BufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;					//// � ���۷� ���Ǵ���
	ZeroMemory(&SubresourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	SubresourceData.pSysMem = &index.at(0);
	if (FAILED(CreateBufferResult = g_pDevice->CreateBuffer(&BufferDesc, &SubresourceData, &m_pIndexBuffer))) return false;
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma endregion CreateBuffer

	CreateConstantBuffer();
	return true;
}