#include "MMapNode.h"
#include "M3DHeightMap.h"

bool MMapNode::CreateMidIndexBuffer(int XCount)
{
	for (int i = 1; i < m_iIndex.size(); i++)
	{
		vector<DWORD> list[4];
		int Count = pow(2, (m_iIndex.size() - i - 1));
		int size = pow(2, i);

		//int flag = m_iIndex[i][j * 6];
		for (int j = 0; j < Count; j++)
		{
			DWORD midi[3];
			midi[0] = m_iIndex[i][0] + (size * j);
			midi[1] = midi[0] + size;
			midi[2] = (midi[0] + midi[1]) / 2;
			list[UP_].push_back(midi[0]);
			list[UP_].push_back(midi[1]);
			list[UP_].push_back(midi[2]);
		}
		for (int j = 0; j < Count; j++)
		{
			DWORD midi[3];
			midi[0] = m_iIndex[i][0] + (Count * size) + (j * size * XCount);
			midi[1] = midi[0] + (XCount * size);
			midi[2] = (midi[0] + midi[1]) / 2;
			list[RIGHT_].push_back(midi[0]);
			list[RIGHT_].push_back(midi[1]);
			list[RIGHT_].push_back(midi[2]);
		}
		for (int j = 0; j < Count; j++)
		{
			DWORD midi[3];
			midi[0] = m_iIndex[i][0] + (Count * size * XCount) + (size * j);
			midi[1] = midi[0] + size;
			midi[2] = (midi[0] + midi[1]) / 2;
			list[DOWN_].push_back(midi[0]);
			list[DOWN_].push_back(midi[1]);
			list[DOWN_].push_back(midi[2]);
		}
		for (int j = 0; j < Count; j++)
		{
			DWORD midi[3];
			midi[0] = m_iIndex[i][0] + (j * size * XCount);
			midi[1] = midi[0] + (XCount * size);
			midi[2] = (midi[0] + midi[1]) / 2;
			list[LEFT_].push_back(midi[0]);
			list[LEFT_].push_back(midi[1]);
			list[LEFT_].push_back(midi[2]);
		}
		m_iMiddleIndex[UP_].push_back(list[UP_]);
		m_iMiddleIndex[RIGHT_].push_back(list[RIGHT_]);
		m_iMiddleIndex[DOWN_].push_back(list[DOWN_]);
		m_iMiddleIndex[LEFT_].push_back(list[LEFT_]);
	}

	D3D11_BUFFER_DESC BufferDesc;				//// 공용 버퍼 데스크
	D3D11_SUBRESOURCE_DATA SubresourceData;		//// 공용 리소스 데이터
	HRESULT CreateBufferResult;					//// 공용 결과값
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < m_iMiddleIndex[i].size(); j++)
		{
			ID3D11Buffer* buffer;
			ZeroMemory(&BufferDesc, sizeof(D3D11_BUFFER_DESC));
			BufferDesc.ByteWidth = sizeof(DWORD) * m_iMiddleIndex[i][j].size();
			BufferDesc.Usage = D3D11_USAGE_DEFAULT;
			BufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
			ZeroMemory(&SubresourceData, sizeof(D3D11_SUBRESOURCE_DATA));
			SubresourceData.pSysMem = &m_iMiddleIndex[i][j].at(0);
			if (FAILED(CreateBufferResult = g_pDevice->CreateBuffer(&BufferDesc, &SubresourceData, &buffer))) return false;
			m_pMidIndexBuffer[i].push_back(buffer);
		}
	}
	return true;
}

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



bool MMapNode::Render()
{
	if (g_bIsLOD)
	{
		g_pImmediateContext->IASetIndexBuffer(m_pIndexBuffer[m_dwCurLevel], DXGI_FORMAT_R32_UINT, 0);
		int count = m_iIndex[m_dwCurLevel].size();
		g_pImmediateContext->DrawIndexed(count, 0, 0);
		if (m_dwCurLevel != m_iIndex.size() - 1)
		{
			for (int i = 0; i < 4; i++)
			{
				if (m_pNeighborNode[i] != nullptr)
				{
					if (m_pNeighborNode[i]->m_dwCurLevel > m_dwCurLevel)
					{
						g_pImmediateContext->IASetIndexBuffer(m_pMidIndexBuffer[i][m_dwCurLevel], DXGI_FORMAT_R32_UINT, 0);
						g_pImmediateContext->DrawIndexed(m_iMiddleIndex[i][m_dwCurLevel].size(), 0, 0);
					}
				}
			}
		}
	}
	else
	{
		g_pImmediateContext->IASetIndexBuffer(m_pIndexBuffer[0], DXGI_FORMAT_R32_UINT, 0);
		int count = m_iIndex[0].size();
		g_pImmediateContext->DrawIndexed(count, 0, 0);
	}
	return true;
}

bool MMapNode::Release()
{
	for (int i = 0; i < m_pIndexBuffer.size(); i++)
	{
		SAFE_RELEASE(m_pIndexBuffer[i]);
	}
	for (int i = 0; i < m_pMidIndexBuffer[0].size(); i++)
	{
		SAFE_RELEASE(m_pMidIndexBuffer[0][i]);
		SAFE_RELEASE(m_pMidIndexBuffer[1][i]);
		SAFE_RELEASE(m_pMidIndexBuffer[2][i]);
		SAFE_RELEASE(m_pMidIndexBuffer[3][i]);
	}
#if defined(DEBUG) || defined(_DEBUG)
	SAFE_RELEASE(m_Box);
#endif // DEBUG
	
	return true;
}

MMapNode::MMapNode()
{
	m_dwCurLevel = 1;
	for (int i = 0; i < 4; i++)
	{
		m_pNeighborNode[i] = nullptr;
	}
}

MMapNode::~MMapNode()
{

}
