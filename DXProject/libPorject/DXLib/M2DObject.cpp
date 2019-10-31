#include "M2DObject.h"



M2DObject::M2DObject()
{
	m_VertexShaderID = VS2D;
}

M2DObject::~M2DObject()
{
}

bool M2DObject::Init()
{
	m_pVertexBuffer = NULL;
	m_pIndexBuffer = NULL;
	m_pConstantBuffer = NULL;

	I_MaterialMgr[MaterialID]->m_PixelShaderID = PS2D;

	m_ScreenPosition.x = 0;
	m_ScreenPosition.y = 0;
	m_ScreenRotation = 0;
	m_ScreenScale.x = 1;
	m_ScreenScale.y = 1;

	I_MaterialMgr[MaterialID]->m_BlendStateID = MAlphaBlend;
	I_MaterialMgr[MaterialID]->m_SamplerStateID = MWrapLinear;
	I_MaterialMgr[MaterialID]->m_DepthStencilStateID = MDepthEnable;
	return true;
}

bool M2DObject::Release()
{
	return true;
}

bool M2DObject::Frame()
{
	D3D11_MAPPED_SUBRESOURCE MappedResource;
	g_pImmediateContext->Map(m_pConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedResource);
	CONSTANT_2DOBJ* pConstantOBJ = (CONSTANT_2DOBJ*)MappedResource.pData; // 주소값을 바인딩
	pConstantOBJ->ScreenPosition = m_ScreenPosition;
	pConstantOBJ->ScreenRotation = m_ScreenRotation;
	pConstantOBJ->ScreenScale = m_ScreenScale;
	g_pImmediateContext->Unmap(m_pConstantBuffer, 0);
	return true;
}

bool M2DObject::Render()
{
	///// 스테이트 매니져로 블랜드셋팅을 받아오기 ////
	I_MaterialMgr[MaterialID]->Render();
	g_pImmediateContext->IASetInputLayout(I_VertexShaderMgr.m_LOList[m_VertexShaderID]);		// 레이아웃 셋
	g_pImmediateContext->VSSetShader(I_VertexShaderMgr.m_VSList[m_VertexShaderID], NULL, 0);
	UINT stride = sizeof(SVERTEX);		//// 보폭(카운트할 사이즈 크기)
	UINT offset = 0;					//// 시작 위치
	g_pImmediateContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);
	g_pImmediateContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	g_pImmediateContext->VSSetConstantBuffers(1, 1, &m_pConstantBuffer);

	/////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////
	g_pImmediateContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	g_pImmediateContext->DrawIndexed(6, 0, 0);
	return true;
}

bool M2DObject::Load(M_STR newfilename)
{
	if (m_pVertexBuffer != NULL) return false;

	if (!I_MaterialMgr[MaterialID]->Load(newfilename, DIFFUSE)) return false;

	//// 디퓨즈맵 로딩이 완료되면 넓이와 높이값을 가져와 버텍스버퍼 생성 ////
	MSize iSize = I_TextureMgr[I_MaterialMgr[MaterialID]->Diffuse]->GetTextureSize();
	SVERTEX vertices[4];
	vertices[0] = SVERTEX(-(iSize.w) / 2.0f, iSize.h / 2.0f, 0.0f, 0.0f, 0.0f);
	vertices[1] = SVERTEX(iSize.w / 2.0f, iSize.h / 2.0f, 0.0f, 1.0f, 0.0f);
	vertices[2] = SVERTEX(iSize.w / 2.0f, -(iSize.h) / 2.0f, 0.0f, 1.0f, 1.0f);
	vertices[3] = SVERTEX(-(iSize.w) / 2.0f, -(iSize.h) / 2.0f, 0.0f, 0.0f, 1.0f);
	DWORD IndexValve[] =
	{
		1,2,0,
		2,3,0,
	};

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	D3D11_BUFFER_DESC BufferDesc;				//// 공용 버퍼 데스크
	D3D11_SUBRESOURCE_DATA SubresourceData;		//// 공용 리소스 데이터
	HRESULT CreateBufferResult;					//// 공용 결과값
	///// 버텍스 버퍼 ///////////////////////////////////////////////////////////////////////////////////////////////////
	ZeroMemory(&BufferDesc, sizeof(D3D11_BUFFER_DESC));
	BufferDesc.ByteWidth = sizeof(SVERTEX) * _countof(vertices);	//// 사이즈 (Byte단위)
	BufferDesc.Usage = D3D11_USAGE_DEFAULT;							//// 사용모드 (DEFAULT는 그래픽카드에 생성)
	BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;				//// 어떤 버퍼로 사용되는지
	ZeroMemory(&SubresourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	SubresourceData.pSysMem = vertices;
	if (FAILED(CreateBufferResult = g_pDevice->CreateBuffer(&BufferDesc, &SubresourceData, &m_pVertexBuffer))) return false;
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//// 인덱스 버퍼 /////////////////////////////////////////////////////////////////////////////////////////////////////
	ZeroMemory(&BufferDesc, sizeof(D3D11_BUFFER_DESC));
	BufferDesc.ByteWidth = sizeof(DWORD) * _countof(IndexValve);	//// 사이즈 (Byte단위)
	BufferDesc.Usage = D3D11_USAGE_DEFAULT;							//// 사용모드 (DEFAULT는 그래픽카드에 생성)
	BufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;					//// 어떤 버퍼로 사용되는지
	ZeroMemory(&SubresourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	SubresourceData.pSysMem = IndexValve;
	if (FAILED(CreateBufferResult = g_pDevice->CreateBuffer(&BufferDesc, &SubresourceData, &m_pIndexBuffer))) return false;
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//// 상수 버퍼 ///////////////////////////////////////////////////////////////////////////////////////////////////////								
	ZeroMemory(&BufferDesc, sizeof(D3D11_BUFFER_DESC));
	BufferDesc.ByteWidth = sizeof(CONSTANT_2DOBJ);
	BufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	BufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	BufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	if (FAILED(CreateBufferResult = g_pDevice->CreateBuffer(&BufferDesc, NULL, &m_pConstantBuffer))) return false;
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	return true;
}

bool M2DObject::Create(UINT width, UINT height)
{
	if (m_pVertexBuffer != NULL) return false;
	SVERTEX vertices[4];
	vertices[0] = SVERTEX(-(width / 2.0f), height / 2.0f, 0.0f, 0.0f, 0.0f);
	vertices[1] = SVERTEX(width / 2.0f, height / 2.0f, 0.0f, 1.0f, 0.0f);
	vertices[2] = SVERTEX(width / 2.0f, -(height / 2.0f), 0.0f, 1.0f, 1.0f);
	vertices[3] = SVERTEX(-(width / 2.0f), -(height / 2.0f), 0.0f, 0.0f, 1.0f);
	DWORD IndexValve[] =
	{
		1,2,0,
		2,3,0,
	};

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	D3D11_BUFFER_DESC BufferDesc;				//// 공용 버퍼 데스크
	D3D11_SUBRESOURCE_DATA SubresourceData;		//// 공용 리소스 데이터
	HRESULT CreateBufferResult;					//// 공용 결과값
	///// 버텍스 버퍼 ///////////////////////////////////////////////////////////////////////////////////////////////////
	ZeroMemory(&BufferDesc, sizeof(D3D11_BUFFER_DESC));
	BufferDesc.ByteWidth = sizeof(SVERTEX) * _countof(vertices);	//// 사이즈 (Byte단위)
	BufferDesc.Usage = D3D11_USAGE_DEFAULT;							//// 사용모드 (DEFAULT는 그래픽카드에 생성)
	BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;				//// 어떤 버퍼로 사용되는지
	ZeroMemory(&SubresourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	SubresourceData.pSysMem = vertices;
	if (FAILED(CreateBufferResult = g_pDevice->CreateBuffer(&BufferDesc, &SubresourceData, &m_pVertexBuffer))) return false;
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//// 인덱스 버퍼 /////////////////////////////////////////////////////////////////////////////////////////////////////
	ZeroMemory(&BufferDesc, sizeof(D3D11_BUFFER_DESC));
	BufferDesc.ByteWidth = sizeof(DWORD) * _countof(IndexValve);	//// 사이즈 (Byte단위)
	BufferDesc.Usage = D3D11_USAGE_DEFAULT;							//// 사용모드 (DEFAULT는 그래픽카드에 생성)
	BufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;					//// 어떤 버퍼로 사용되는지
	ZeroMemory(&SubresourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	SubresourceData.pSysMem = IndexValve;
	if (FAILED(CreateBufferResult = g_pDevice->CreateBuffer(&BufferDesc, &SubresourceData, &m_pIndexBuffer))) return false;
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//// 상수 버퍼 ///////////////////////////////////////////////////////////////////////////////////////////////////////								
	ZeroMemory(&BufferDesc, sizeof(D3D11_BUFFER_DESC));
	BufferDesc.ByteWidth = sizeof(CONSTANT_2DOBJ);
	BufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	BufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	BufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	if (FAILED(CreateBufferResult = g_pDevice->CreateBuffer(&BufferDesc, NULL, &m_pConstantBuffer))) return false;
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	return true;
}

void M2DObject::SetPosition(MPoint pos)
{
	m_ScreenPosition = pos;
}

void M2DObject::SetRotation(float rot)
{
	m_ScreenRotation = rot;
}

void M2DObject::SetScale(MPoint scl)
{
	m_ScreenScale = scl;
}

void M2DObject::ModifyPosition(MPoint pos)
{
	m_ScreenPosition += pos;
}

void M2DObject::ModifyRotation(float rot)
{
	m_ScreenRotation += rot;
}

void M2DObject::ModifyScale(MPoint scl)
{
	m_ScreenScale += scl;
}