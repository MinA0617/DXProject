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
	///// 스테이트 매니져로 블랜드셋팅을 받아오기 ////

	I_MaterialMgr[MaterialID]->Render();

	UINT stride = sizeof(SPLINE_VERTEX);		//// 보폭(카운트할 사이즈 크기)
	UINT offset = 0;					//// 시작 위치
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
	//// 나중에 파일이름으로 불러오는것으로 바꾸기 ////
	///////////////////////////////////////////////////
	///////////////////////////////////////////////////
	///////////////////////////////////////////////////
	///////////////////////////////////////////////////
	///////////////////////////////////////////////////
	///////////////////////////////////////////////////
	///////////////////////////////////////////////////


	//D3D11_BUFFER_DESC BufferDesc;				//// 공용 버퍼 데스크
	//D3D11_SUBRESOURCE_DATA SubresourceData;		//// 공용 리소스 데이터
	//HRESULT CreateBufferResult;					//// 공용 결과값
	//	///// 버텍스 버퍼 ///////////////////////////////////////////////////////////////////////////////////////////////////
	//ZeroMemory(&BufferDesc, sizeof(D3D11_BUFFER_DESC));
	//BufferDesc.ByteWidth = sizeof(SPLINE_VERTEX) * iCount;	//// 사이즈 (Byte단위)
	//BufferDesc.Usage = D3D11_USAGE_DEFAULT;							//// 사용모드 (DEFAULT는 그래픽카드에 생성)
	//BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;				//// 어떤 버퍼로 사용되는지
	//ZeroMemory(&SubresourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	//SubresourceData.pSysMem = data;
	//if (FAILED(CreateBufferResult = g_pDevice->CreateBuffer(&BufferDesc, &SubresourceData, &m_pVertexBuffer))) return false;
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	////// 인덱스 버퍼 /////////////////////////////////////////////////////////////////////////////////////////////////////
	//ZeroMemory(&BufferDesc, sizeof(D3D11_BUFFER_DESC));
	//BufferDesc.ByteWidth = sizeof(DWORD) * m_iIndexCount;	//// 사이즈 (Byte단위)
	//BufferDesc.Usage = D3D11_USAGE_DEFAULT;							//// 사용모드 (DEFAULT는 그래픽카드에 생성)
	//BufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;					//// 어떤 버퍼로 사용되는지
	//ZeroMemory(&SubresourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	//SubresourceData.pSysMem = index3D;
	//if (FAILED(CreateBufferResult = g_pDevice->CreateBuffer(&BufferDesc, &SubresourceData, &m_pIndexBuffer))) return false;
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	return true;
}