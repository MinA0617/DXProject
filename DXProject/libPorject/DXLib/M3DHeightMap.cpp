#include "M3DHeightMap.h"



D3DXVECTOR3 M3DHeightMap::ComputeFaceNormal(DWORD i0, DWORD i1, DWORD i2)
{
	D3DXVECTOR3 vNormal;
	float3 v0 = m_VertexList[i1].p - m_VertexList[i0].p;
	float3 v1 = m_VertexList[i2].p - m_VertexList[i0].p;
	D3DXVECTOR3 vv0(v0.x, v0.y, v0.z);
	D3DXVECTOR3 vv1(v1.x, v1.y, v1.z);
	D3DXVec3Cross(&vNormal, &vv0, &vv1);
	D3DXVec3Normalize(&vNormal, &vNormal);
	return vNormal;
}

bool M3DHeightMap::ComputeVertexNormal(DWORD i)
{
	D3DXVECTOR3 normal(0, 0, 0);
	int iSize = m_VertexFaceIndex[i].size();
	if (iSize == 0) return false;
	for (int j = 0; j < m_VertexFaceIndex[i].size(); j++)
	{
		normal += m_FaceNormal[m_VertexFaceIndex[i][j]];
	}
	D3DXVec3Normalize(&normal, &normal);
	m_VertexList[i].n = float3(normal);
	return true;
}

bool M3DHeightMap::ComputeNormal()
{
	m_VertexFaceIndex.resize(m_VertexList.size());
	m_FaceNormal.resize(m_IndexList.size() / 3);
	int iSize = m_IndexList.size();
	int FaceIndex = 0;
	for (int i = 0; i < iSize; i = i =i+3)
	{
		D3DXVECTOR3 fNormal = ComputeFaceNormal(m_IndexList[i], m_IndexList[i + 1], m_IndexList[i + 2]);
		m_VertexFaceIndex[m_IndexList[i]].push_back(FaceIndex);
		m_VertexFaceIndex[m_IndexList[i + 1]].push_back(FaceIndex);
		m_VertexFaceIndex[m_IndexList[i + 2]].push_back(FaceIndex);
		m_FaceNormal[FaceIndex] = fNormal;
		FaceIndex++;
	}
	for (int i = 0; i < m_VertexList.size(); i++)
	{
		ComputeVertexNormal(i);
		m_VertexList[i].tv = float3(0, 1, 0);
	}
	return true;
}

bool M3DHeightMap::CreateBuffer()
{
	if (m_VertexList.size() == 0 || m_IndexList.size() == 0)
	{
		return false;
	}
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	D3D11_BUFFER_DESC BufferDesc;				//// 공용 버퍼 데스크
	D3D11_SUBRESOURCE_DATA SubresourceData;		//// 공용 리소스 데이터
	HRESULT CreateBufferResult;					//// 공용 결과값
	///// 버텍스 버퍼 ///////////////////////////////////////////////////////////////////////////////////////////////////
	ZeroMemory(&BufferDesc, sizeof(D3D11_BUFFER_DESC));
	BufferDesc.ByteWidth = sizeof(MVERTEX) * m_VertexList.size();		//// 사이즈 (Byte단위)
	BufferDesc.Usage = D3D11_USAGE_DEFAULT;							//// 사용모드 (DEFAULT는 그래픽카드에 생성)
	BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;				//// 어떤 버퍼로 사용되는지
	ZeroMemory(&SubresourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	SubresourceData.pSysMem = &m_VertexList.at(0);
	if (FAILED(CreateBufferResult = g_pDevice->CreateBuffer(&BufferDesc, &SubresourceData, &m_pVertexBuffer))) return false;
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//// 인덱스 버퍼 /////////////////////////////////////////////////////////////////////////////////////////////////////
	ZeroMemory(&BufferDesc, sizeof(D3D11_BUFFER_DESC));
	BufferDesc.ByteWidth = sizeof(DWORD) * m_IndexList.size();			//// 사이즈 (Byte단위)
	BufferDesc.Usage = D3D11_USAGE_DEFAULT;							//// 사용모드 (DEFAULT는 그래픽카드에 생성)
	BufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;					//// 어떤 버퍼로 사용되는지
	ZeroMemory(&SubresourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	SubresourceData.pSysMem = &m_IndexList.at(0);
	if (FAILED(CreateBufferResult = g_pDevice->CreateBuffer(&BufferDesc, &SubresourceData, &m_pIndexBuffer))) return false;
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	return true;
}

bool M3DHeightMap::CreateVertex(vector<float>& list)
{
	int count = 0;
	float xminsize = -m_iXCount * m_fLeafSize / 2;
	float zminsize = -m_iZCount * m_fLeafSize / 2;

	m_VertexList.reserve(m_iZCount * m_iXCount);
	for (int i = 0; i < m_iZCount; i++)
	{
		for (int j = 0; j < m_iXCount; j++)
		{
			MVERTEX ver;
			ver.p = float3(xminsize + (j * m_fLeafSize), list[count], zminsize + (i * m_fLeafSize));
			ver.t = float3(j / m_iXCount, i / m_iZCount, 0);
			count++;
			m_VertexList.push_back(ver);
		}
	}
	int incount = 0;
	for (int i = 0; i < m_iZCount - 1; i++)
	{
		for (int j = 0; j < m_iXCount - 1; j++)
		{
			DWORD index[6];
			index[0] = incount;
			index[1] = incount + m_iXCount;
			index[2] = incount + 1;
			index[3] = incount + 1;
			index[4] = incount + m_iXCount;
			index[5] = incount + m_iXCount + 1;
			m_IndexList.push_back(index[0]);
			m_IndexList.push_back(index[1]);
			m_IndexList.push_back(index[2]);
			m_IndexList.push_back(index[3]);
			m_IndexList.push_back(index[4]);
			m_IndexList.push_back(index[5]);
			incount++;
		}
		incount++;
	}
	ComputeNormal();
	CreateBuffer();
	return false;
}

bool M3DHeightMap::Create(M_STR filename, float size)
{
	if (size == 0) return false;
	m_fLeafSize = size;
	vector<float>	fList;
	//ID3D11ShaderResourceView* pTexture = NULL;
	ID3D11Resource *pTexture = NULL;
	ID3D11Texture2D *pTexture2D = NULL;
	HRESULT TextureLoadResult;

	D3DX11_IMAGE_INFO ImageInfo;
	D3DX11GetImageInfoFromFile(filename.c_str(), NULL, &ImageInfo, &TextureLoadResult);
	if (FAILED(TextureLoadResult)) return false;

	D3DX11_IMAGE_LOAD_INFO loadInfo;
	ZeroMemory(&loadInfo, sizeof(D3DX11_IMAGE_LOAD_INFO));
	loadInfo.Usage = D3D11_USAGE_STAGING;
	loadInfo.CpuAccessFlags = D3D11_CPU_ACCESS_WRITE | D3D11_CPU_ACCESS_READ;
	//loadInfo.Format = DXGI_FORMAT_FROM_FILE;
	loadInfo.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	loadInfo.pSrcInfo = &ImageInfo;

	//// 디바이스객체, 파일이름, 마테리얼속성, 펌프인터페이스, 텍스쳐리소스, 결과값,
	TextureLoadResult = D3DX11CreateTextureFromFile(g_pDevice, filename.c_str(), &loadInfo, NULL, &pTexture, NULL);
	if (FAILED(TextureLoadResult)) return false;
	TextureLoadResult = pTexture->QueryInterface(__uuidof(ID3D11Texture2D), (LPVOID*)&pTexture2D);
	if (FAILED(TextureLoadResult)) return false;
	SAFE_RELEASE(pTexture);
	D3D11_TEXTURE2D_DESC desc;
	pTexture2D->GetDesc(&desc);
	fList.resize(desc.Height*desc.Width);
	if (pTexture2D)
	{
		D3D11_MAPPED_SUBRESOURCE MappedFaceDest;
		if (SUCCEEDED(g_pImmediateContext->Map((ID3D11Resource*)pTexture2D, D3D11CalcSubresource(0, 0, 1), D3D11_MAP_READ, 0, &MappedFaceDest)))
		{
			UCHAR* pTexels = (UCHAR*)MappedFaceDest.pData;
			//MVERTEX	v;
			for (UINT row = 0; row < desc.Height; row++)
			{
				UINT rowStart = row * MappedFaceDest.RowPitch;
				for (UINT col = 0; col < desc.Width; col++)
				{
					UINT colStart = col * 4;
					UINT uRed = pTexels[rowStart + colStart + 0];
					fList[row * desc.Width + col] = uRed;	/// DWORD이므로 pitch/4	
					if (uRed > m_fYmax) m_fYmax = uRed;
				}
			}
			g_pImmediateContext->Unmap(pTexture2D, D3D11CalcSubresource(0, 0, 1));
		}
	}

	m_iZCount = desc.Height;
	m_iXCount = desc.Width;
	SAFE_RELEASE(pTexture2D);
	CreateVertex(fList);

	TCHAR Drive[MAX_PATH] = { 0, };
	TCHAR Dir[MAX_PATH] = { 0, };
	TCHAR Name[MAX_PATH] = { 0, };
	TCHAR Ext[MAX_PATH] = { 0, };
	_tsplitpath_s(filename.c_str(), Drive, Dir, Name, Ext);
	m_name = Name;
	return true;
}

bool M3DHeightMap::Init()
{
	m_VertexShaderID = VS3D;
	CreateConstantBuffer();
	return true;
}

bool M3DHeightMap::Frame()
{
	PreFrame();
	return true;
}

bool M3DHeightMap::Render()
{
	I_MaterialMgr[MaterialID]->Render();
	g_pImmediateContext->VSSetConstantBuffers(2, 1, &m_pConstantBuffer);
	g_pImmediateContext->IASetInputLayout(I_VertexShaderMgr.m_LOList[m_VertexShaderID]);		// 레이아웃 셋
	g_pImmediateContext->VSSetShader(I_VertexShaderMgr.m_VSList[m_VertexShaderID], NULL, 0);
	UINT stride = sizeof(MVERTEX);		//// 보폭(카운트할 사이즈 크기)
	UINT offset = 0;
	g_pImmediateContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);
	g_pImmediateContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	g_pImmediateContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	g_pImmediateContext->DrawIndexed(m_IndexList.size(), 0, 0);
	return true;
}

bool M3DHeightMap::Release()
{
	SAFE_RELEASE(m_pConstantBuffer);
	SAFE_RELEASE(m_pVertexBuffer);
	SAFE_RELEASE(m_pIndexBuffer);
	return true;
}

M3DHeightMap::M3DHeightMap()
{
}


M3DHeightMap::~M3DHeightMap()
{
}
