#include "M3DSpline.h"



M3DSpline::M3DSpline()
{
}


M3DSpline::~M3DSpline()
{
}

bool M3DSpline::Init()
{

	I_MaterialMgr[MaterialID]->m_VertexShaderID = VSSPLINE;
	I_MaterialMgr[MaterialID]->m_PixelShaderID = PSSPLINE;

	return true;
}

bool M3DSpline::Release()
{
	SAFE_RELEASE(m_pVertexBuffer);
	SAFE_RELEASE(m_pIndexBuffer);
	SAFE_RELEASE(m_pConstantBuffer);
	return true;
}

bool M3DSpline::Frame()
{
	PreFrame();
	return true;
}

bool M3DSpline::Render()
{
	///// ������Ʈ �Ŵ����� ��������� �޾ƿ��� ////

	I_MaterialMgr[MaterialID]->Render();

	UINT stride = sizeof(SPLINE_VERTEX);		//// ����(ī��Ʈ�� ������ ũ��)
	UINT offset = 0;					//// ���� ��ġ
	g_pImmediateContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);
	g_pImmediateContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	g_pImmediateContext->VSSetConstantBuffers(1, 1, &m_pConstantBuffer);

	/////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////
	g_pImmediateContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINELIST);
	//g_pImmediateContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_POINTLIST);
	g_pImmediateContext->DrawIndexed(m_iIndexCount, 0, 0);
	return true;
}

bool M3DSpline::Load(SPLINE_VERTEX* data, UINT iCount)
{
	///////////////////////////////////////////////////
	///////////////////////////////////////////////////
	///////////////////////////////////////////////////
	///////////////////////////////////////////////////
	///////////////////////////////////////////////////
	///////////////////////////////////////////////////
	///////////////////////////////////////////////////
	//// ���߿� �����̸����� �ҷ����°����� �ٲٱ� ////
	///////////////////////////////////////////////////
	///////////////////////////////////////////////////
	///////////////////////////////////////////////////
	///////////////////////////////////////////////////
	///////////////////////////////////////////////////
	///////////////////////////////////////////////////
	///////////////////////////////////////////////////


	//D3D11_BUFFER_DESC BufferDesc;				//// ���� ���� ����ũ
	//D3D11_SUBRESOURCE_DATA SubresourceData;		//// ���� ���ҽ� ������
	//HRESULT CreateBufferResult;					//// ���� �����
	//	///// ���ؽ� ���� ///////////////////////////////////////////////////////////////////////////////////////////////////
	//ZeroMemory(&BufferDesc, sizeof(D3D11_BUFFER_DESC));
	//BufferDesc.ByteWidth = sizeof(SPLINE_VERTEX) * iCount;	//// ������ (Byte����)
	//BufferDesc.Usage = D3D11_USAGE_DEFAULT;							//// ����� (DEFAULT�� �׷���ī�忡 ����)
	//BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;				//// � ���۷� ���Ǵ���
	//ZeroMemory(&SubresourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	//SubresourceData.pSysMem = data;
	//if (FAILED(CreateBufferResult = g_pDevice->CreateBuffer(&BufferDesc, &SubresourceData, &m_pVertexBuffer))) return false;
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	////// �ε��� ���� /////////////////////////////////////////////////////////////////////////////////////////////////////
	//ZeroMemory(&BufferDesc, sizeof(D3D11_BUFFER_DESC));
	//BufferDesc.ByteWidth = sizeof(DWORD) * m_iIndexCount;	//// ������ (Byte����)
	//BufferDesc.Usage = D3D11_USAGE_DEFAULT;							//// ����� (DEFAULT�� �׷���ī�忡 ����)
	//BufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;					//// � ���۷� ���Ǵ���
	//ZeroMemory(&SubresourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	//SubresourceData.pSysMem = index3D;
	//if (FAILED(CreateBufferResult = g_pDevice->CreateBuffer(&BufferDesc, &SubresourceData, &m_pIndexBuffer))) return false;
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	return true;
}