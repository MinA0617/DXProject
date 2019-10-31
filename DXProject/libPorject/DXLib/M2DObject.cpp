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
	CONSTANT_2DOBJ* pConstantOBJ = (CONSTANT_2DOBJ*)MappedResource.pData; // �ּҰ��� ���ε�
	pConstantOBJ->ScreenPosition = m_ScreenPosition;
	pConstantOBJ->ScreenRotation = m_ScreenRotation;
	pConstantOBJ->ScreenScale = m_ScreenScale;
	g_pImmediateContext->Unmap(m_pConstantBuffer, 0);
	return true;
}

bool M2DObject::Render()
{
	///// ������Ʈ �Ŵ����� ��������� �޾ƿ��� ////
	I_MaterialMgr[MaterialID]->Render();
	g_pImmediateContext->IASetInputLayout(I_VertexShaderMgr.m_LOList[m_VertexShaderID]);		// ���̾ƿ� ��
	g_pImmediateContext->VSSetShader(I_VertexShaderMgr.m_VSList[m_VertexShaderID], NULL, 0);
	UINT stride = sizeof(SVERTEX);		//// ����(ī��Ʈ�� ������ ũ��)
	UINT offset = 0;					//// ���� ��ġ
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

	//// ��ǻ��� �ε��� �Ϸ�Ǹ� ���̿� ���̰��� ������ ���ؽ����� ���� ////
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
	D3D11_BUFFER_DESC BufferDesc;				//// ���� ���� ����ũ
	D3D11_SUBRESOURCE_DATA SubresourceData;		//// ���� ���ҽ� ������
	HRESULT CreateBufferResult;					//// ���� �����
	///// ���ؽ� ���� ///////////////////////////////////////////////////////////////////////////////////////////////////
	ZeroMemory(&BufferDesc, sizeof(D3D11_BUFFER_DESC));
	BufferDesc.ByteWidth = sizeof(SVERTEX) * _countof(vertices);	//// ������ (Byte����)
	BufferDesc.Usage = D3D11_USAGE_DEFAULT;							//// ����� (DEFAULT�� �׷���ī�忡 ����)
	BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;				//// � ���۷� ���Ǵ���
	ZeroMemory(&SubresourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	SubresourceData.pSysMem = vertices;
	if (FAILED(CreateBufferResult = g_pDevice->CreateBuffer(&BufferDesc, &SubresourceData, &m_pVertexBuffer))) return false;
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//// �ε��� ���� /////////////////////////////////////////////////////////////////////////////////////////////////////
	ZeroMemory(&BufferDesc, sizeof(D3D11_BUFFER_DESC));
	BufferDesc.ByteWidth = sizeof(DWORD) * _countof(IndexValve);	//// ������ (Byte����)
	BufferDesc.Usage = D3D11_USAGE_DEFAULT;							//// ����� (DEFAULT�� �׷���ī�忡 ����)
	BufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;					//// � ���۷� ���Ǵ���
	ZeroMemory(&SubresourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	SubresourceData.pSysMem = IndexValve;
	if (FAILED(CreateBufferResult = g_pDevice->CreateBuffer(&BufferDesc, &SubresourceData, &m_pIndexBuffer))) return false;
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//// ��� ���� ///////////////////////////////////////////////////////////////////////////////////////////////////////								
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
	D3D11_BUFFER_DESC BufferDesc;				//// ���� ���� ����ũ
	D3D11_SUBRESOURCE_DATA SubresourceData;		//// ���� ���ҽ� ������
	HRESULT CreateBufferResult;					//// ���� �����
	///// ���ؽ� ���� ///////////////////////////////////////////////////////////////////////////////////////////////////
	ZeroMemory(&BufferDesc, sizeof(D3D11_BUFFER_DESC));
	BufferDesc.ByteWidth = sizeof(SVERTEX) * _countof(vertices);	//// ������ (Byte����)
	BufferDesc.Usage = D3D11_USAGE_DEFAULT;							//// ����� (DEFAULT�� �׷���ī�忡 ����)
	BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;				//// � ���۷� ���Ǵ���
	ZeroMemory(&SubresourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	SubresourceData.pSysMem = vertices;
	if (FAILED(CreateBufferResult = g_pDevice->CreateBuffer(&BufferDesc, &SubresourceData, &m_pVertexBuffer))) return false;
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//// �ε��� ���� /////////////////////////////////////////////////////////////////////////////////////////////////////
	ZeroMemory(&BufferDesc, sizeof(D3D11_BUFFER_DESC));
	BufferDesc.ByteWidth = sizeof(DWORD) * _countof(IndexValve);	//// ������ (Byte����)
	BufferDesc.Usage = D3D11_USAGE_DEFAULT;							//// ����� (DEFAULT�� �׷���ī�忡 ����)
	BufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;					//// � ���۷� ���Ǵ���
	ZeroMemory(&SubresourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	SubresourceData.pSysMem = IndexValve;
	if (FAILED(CreateBufferResult = g_pDevice->CreateBuffer(&BufferDesc, &SubresourceData, &m_pIndexBuffer))) return false;
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//// ��� ���� ///////////////////////////////////////////////////////////////////////////////////////////////////////								
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