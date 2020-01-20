#include "MSkinMesh.h"
#include "MSkeleton.h"


MSkinMesh::MSkinMesh()
{
	m_VertexShaderID = VSCHARACTER;
}

MSkinMesh::~MSkinMesh()
{
}

bool MSkinMesh::SetSkeletonAndCreateData(MSkeleton * skt, vector<CVERTEX>& OldData)
{
	vector<MVERTEX>		mv;
	vector<SKINDATA>	ms;
	if (OldData.size() == 0)
	{
		return false;
	}
	for (auto temp : OldData)
	{
		MVERTEX newv;
		newv.p = temp.p;
		newv.n = temp.n;
		newv.t = temp.t;
		newv.tv = temp.tv;

		SKINDATA news;
		ZeroMemory(&news, sizeof(SKINDATA));
		news.m_NumWeight = temp.bp.m_NumWeight;
		for (int i = 0; i < news.m_NumWeight; i++)
		{
			news.m_Weight[i] = temp.bp.m_Weight[i];
			news.m_ID[i] = skt->NumofBone(temp.bp.m_ID[i]);
		}
		mv.push_back(newv);
		ms.push_back(news);
	}
	//OldData.clear();
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	D3D11_BUFFER_DESC BufferDesc;				//// ���� ���� ����ũ
	D3D11_SUBRESOURCE_DATA SubresourceData;		//// ���� ���ҽ� ������
	HRESULT CreateBufferResult;					//// ���� �����
	///// ���ؽ� ���� ///////////////////////////////////////////////////////////////////////////////////////////////////
	ZeroMemory(&BufferDesc, sizeof(D3D11_BUFFER_DESC));
	BufferDesc.ByteWidth = sizeof(MVERTEX) * mv.size();		//// ������ (Byte����)
	BufferDesc.Usage = D3D11_USAGE_DEFAULT;							//// ����� (DEFAULT�� �׷���ī�忡 ����)
	BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;				//// � ���۷� ���Ǵ���
	ZeroMemory(&SubresourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	SubresourceData.pSysMem = &mv.at(0);
	if (FAILED(CreateBufferResult = g_pDevice->CreateBuffer(&BufferDesc, &SubresourceData, &m_pVertexBuffer))) return false;
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///// ���ؽ� ���� ///////////////////////////////////////////////////////////////////////////////////////////////////
	ZeroMemory(&BufferDesc, sizeof(D3D11_BUFFER_DESC));
	BufferDesc.ByteWidth = sizeof(SKINDATA) * ms.size();		//// ������ (Byte����)
	BufferDesc.Usage = D3D11_USAGE_DEFAULT;							//// ����� (DEFAULT�� �׷���ī�忡 ����)
	BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;				//// � ���۷� ���Ǵ���
	ZeroMemory(&SubresourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	SubresourceData.pSysMem = &ms.at(0);
	if (FAILED(CreateBufferResult = g_pDevice->CreateBuffer(&BufferDesc, &SubresourceData, &m_pSkinBuffer))) return false;
	return true;
}

bool MSkinMesh::Init()
{
	return true;
}

bool MSkinMesh::Render()
{
	g_pImmediateContext->IASetInputLayout(I_VertexShaderMgr.m_LOList[m_VertexShaderID]);		// ���̾ƿ� ��
	g_pImmediateContext->VSSetShader(I_VertexShaderMgr.m_VSList[m_VertexShaderID], NULL, 0);
	UINT stride = sizeof(MVERTEX);		//// ����(ī��Ʈ�� ������ ũ��)
	UINT offset = 0;
	g_pImmediateContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);
	stride = sizeof(SKINDATA);
	g_pImmediateContext->IASetVertexBuffers(1, 1, &m_pSkinBuffer, &stride, &offset);
	g_pImmediateContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	g_pImmediateContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	g_pImmediateContext->DrawIndexed(m_iIndexCount, 0, 0);
	return true;
}

bool MSkinMesh::Release()
{
	//VertexData.clear();
	//IndexData.clear();
	SAFE_RELEASE(m_pSkinBuffer);
	SAFE_RELEASE(m_pVertexBuffer);
	SAFE_RELEASE(m_pIndexBuffer);
	return true;
}