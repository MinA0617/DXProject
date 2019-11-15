#include "stdafx.h"
#include "MCanvas.h"

//bool MCanvas::Create()
//{
//	if (!I_3DObjectMgr.m_InWorldFiled) return false;
//	m_iSize = I_3DObjectMgr.m_InWorldFiled->ground->m_iCount;
//	HRESULT hr;
//	D3D11_TEXTURE2D_DESC td;
//	ZeroMemory(&td, sizeof(D3D11_TEXTURE2D_DESC));
//	td.Width = m_iSize;
//	td.Height = m_iSize;
//	td.MipLevels = 1;
//	td.SampleDesc.Count = 1;
//	td.SampleDesc.Quality = 0;
//	td.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
//	td.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE | D3D11_CPU_ACCESS_READ;
//	td.BindFlags = D3D11_BIND_SHADER_RESOURCE;
//	td.ArraySize = 1;
//	hr = g_pDevice->CreateTexture2D(&td, NULL, &m_pAlphaTexture);
//}

bool MCanvas::Brushing(UINT layer)
{
	if (layer > 3) return false;
	if (!I_3DObjectMgr.m_InWorldFiled) return false;
	M3DHeightMap* hm = I_3DObjectMgr.m_InWorldFiled->ground;
	if(!hm->m_pAlphaTexture) return false;
	MRAY ray = MSelect::GetScreenRay();
	MTreeNode* pNode = MSelect::CheckNode(I_3DObjectMgr.m_pTree->m_pRootNode, &ray);
	D3DXVECTOR3 center;
	DWORD		index;
	int	XCount = I_3DObjectMgr.m_InWorldFiled->ground->m_iCount;
	if (pNode)
	{
		if (MSelect::CheckTri(pNode, &ray, &center, &index))
		{
			float x = ((hm->m_VertexList[index].p.x) / (hm->m_fLeafSize * hm->m_iCount)) + 0.5;
			float y = ((hm->m_VertexList[index].p.z) / (hm->m_fLeafSize * hm->m_iCount)) + 0.5;

			HRESULT hr;
			ID3D11Resource* AlphaTexture;
			ID3D11Texture2D* AlphaTexture2D;

			hm->m_pAlphaTexture->GetResource(&AlphaTexture);
			hr = AlphaTexture->QueryInterface(__uuidof(ID3D11Texture2D), (LPVOID*)&AlphaTexture2D);
			if (FAILED(hr)) return false;
			//SAFE_RELEASE(AlphaTexture);
			D3D11_TEXTURE2D_DESC desc;
			AlphaTexture2D->GetDesc(&desc);

			D3D11_MAPPED_SUBRESOURCE MappedFaceDest;

			hr = g_pImmediateContext->Map(AlphaTexture, 0, D3D11_MAP_WRITE, 0, &MappedFaceDest);
			hr = g_pImmediateContext->Map((ID3D11Resource*)AlphaTexture2D, D3D11CalcSubresource(0, 0, 1), D3D11_MAP_WRITE, 0, &MappedFaceDest);
			hr = g_pImmediateContext->Map(AlphaTexture2D, 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedFaceDest);
			hr = g_pImmediateContext->Map(AlphaTexture2D, D3D11CalcSubresource(0, 0, 1), D3D11_MAP_WRITE_DISCARD, 0, &MappedFaceDest);
			if (SUCCEEDED(hr))
			{
				UCHAR* pTexels = (UCHAR*)MappedFaceDest.pData;
				UINT flag = ((y * MappedFaceDest.RowPitch) * MappedFaceDest.RowPitch) + (x * MappedFaceDest.RowPitch);
				//MVERTEX	v;
				for (UINT i = 0; i < m_iRadius; i++)
				{
					UINT rowStart = i * MappedFaceDest.RowPitch;
					for (UINT j = 0; j < m_iRadius; j++)
					{
						UINT colStart = (j * 4) + layer;
						pTexels[rowStart + colStart + 0] = 1;
					}
				}
			}
			g_pImmediateContext->Unmap(AlphaTexture2D, D3D11CalcSubresource(0, 0, 1));
		}
		return true;
	}
}

MCanvas::MCanvas()
{
	m_iRadius = 10;
}


MCanvas::~MCanvas()
{
}
