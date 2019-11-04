#include "MMapNode.h"
#include "M3DHeightMap.h"

//bool MMapNode::CreateMidIndexBuffer()
//{
//	for (int i = 0; i < 3; i++)
//	{
//		m_iMiddleIndex[i][0] = m_iIndex[i];
//		m_iMiddleIndex[i][1] = m_iIndex[i + 1];
//		m_iMiddleIndex[i][2] = m_pChildNode[i]->m_iIndex[i + 1];
//	}
//	m_iMiddleIndex[3][0] = m_iIndex[3];
//	m_iMiddleIndex[3][1] = m_iIndex[0];
//	m_iMiddleIndex[3][2] = m_pChildNode[3]->m_iIndex[0];
//	for (int i = 0; i < 4; i++)
//	{
//		m_iMiddleIndex[i][3] = m_iMiddleIndex[i][1];
//		m_iMiddleIndex[i][4] = m_iMiddleIndex[i][0];
//		m_iMiddleIndex[i][5] = m_iMiddleIndex[i][2];
//	}
//
//	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	D3D11_BUFFER_DESC BufferDesc;				//// 공용 버퍼 데스크
//	D3D11_SUBRESOURCE_DATA SubresourceData;		//// 공용 리소스 데이터
//	HRESULT CreateBufferResult;					//// 공용 결과값
//	///// 버텍스 버퍼 ///////////////////////////////////////////////////////////////////////////////////////////////////
//	for (int i = 0; i < 4; i++)
//	{
//		ZeroMemory(&BufferDesc, sizeof(D3D11_BUFFER_DESC));
//		BufferDesc.ByteWidth = sizeof(DWORD) * 6;
//		BufferDesc.Usage = D3D11_USAGE_DYNAMIC;
//		BufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
//		ZeroMemory(&SubresourceData, sizeof(D3D11_SUBRESOURCE_DATA));
//		SubresourceData.pSysMem = m_iMiddleIndex[i];
//		if (FAILED(CreateBufferResult = g_pDevice->CreateBuffer(&BufferDesc, &SubresourceData, &m_pMidIndexBuffer[i]))) return false;
//		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	}
//
//	return true;
//}

bool MMapNode::CreateIndexBuffer()
{
	int iSize = m_iIndex.size();
	for (int i = 0; i < iSize; i++)
	{
		ID3D11Buffer* buffer;
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		D3D11_BUFFER_DESC BufferDesc;				//// 공용 버퍼 데스크
		D3D11_SUBRESOURCE_DATA SubresourceData;		//// 공용 리소스 데이터
		HRESULT CreateBufferResult;					//// 공용 결과값
		///// 버텍스 버퍼 ///////////////////////////////////////////////////////////////////////////////////////////////////
		ZeroMemory(&BufferDesc, sizeof(D3D11_BUFFER_DESC));
		BufferDesc.ByteWidth = sizeof(DWORD) * m_iIndex[i].size();
		BufferDesc.Usage = D3D11_USAGE_DEFAULT;
		BufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		ZeroMemory(&SubresourceData, sizeof(D3D11_SUBRESOURCE_DATA));
		SubresourceData.pSysMem = &m_iIndex[i].at(0);
		if (FAILED(CreateBufferResult = g_pDevice->CreateBuffer(&BufferDesc, &SubresourceData, &buffer))) return false;
		m_pIndexBuffer.push_back(buffer);
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	}
	return true;
}

bool MMapNode::Check(D3DXVECTOR3 & camerapos)
{

	return false;
}

bool MMapNode::Render()
{
	g_pImmediateContext->IASetIndexBuffer(m_pIndexBuffer[m_dwCurLevel], DXGI_FORMAT_R32_UINT, 0);
	g_pImmediateContext->DrawIndexed(m_iIndex[m_dwCurLevel].size(), 0, 0);
	return true;
}

bool MMapNode::Release()
{
	for (int i = 0; i < m_pIndexBuffer.size(); i++)
	{
		SAFE_RELEASE(m_pIndexBuffer[i]);
	}
	return true;
}

MMapNode::MMapNode()
{
	m_dwCurLevel = 1;
	//for (int i = 0; i < 4; i++)
	//{
	//	m_pNeighborNode[i] = nullptr;
	//	m_pChildNode[i] = nullptr;
	//}
}

MMapNode::~MMapNode()
{

}
