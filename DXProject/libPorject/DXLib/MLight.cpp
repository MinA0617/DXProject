#include "MLight.h"



bool MLight::CreateConstantBuffer()
{
	D3D11_BUFFER_DESC BufferDesc;
	HRESULT CreateBufferResult;
	D3D11_SUBRESOURCE_DATA SubresourceData;

	ZeroMemory(&BufferDesc, sizeof(D3D11_BUFFER_DESC));
	BufferDesc.ByteWidth = sizeof(CONSTANT_LIGHT);
	BufferDesc.Usage = D3D11_USAGE_DEFAULT;
	BufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

	ZeroMemory(&SubresourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	SubresourceData.pSysMem = &m_ConstantLigth;

	if (FAILED(CreateBufferResult = g_pDevice->CreateBuffer(&BufferDesc, &SubresourceData, &m_pLightConstantBuffer))) return false;

	BufferDesc.ByteWidth = sizeof(CONSTANT_3DOBJ);
	SubresourceData.pSysMem = &m_ConstantOBJ;

	if (FAILED(CreateBufferResult = g_pDevice->CreateBuffer(&BufferDesc, &SubresourceData, &m_pConstantBuffer))) return false;

	return true;
}

bool MLight::Init()
{
	m_ConstantLigth.m_Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_ConstantLigth.m_fDistance = 100.0f;
	m_ConstantLigth.m_Direction = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	m_ConstantLigth.m_fMultiplier = 1.0f;
	m_ConstantLigth.m_Color = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	m_ConstantLigth.m_fInner = 0.95f;
	m_ConstantLigth.m_fOutner = 1.57f;
	m_ConstantLigth.m_iFlag = SPOT;
	m_ConstantLigth.m_fOffset = 0.0f;
	CreateConstantBuffer();
	return true;
}

bool MLight::Frame()
{
	PreFrame();
	//m_ConstantOBJ.matWorld._41
	m_ConstantLigth.m_Pos.x = m_ConstantOBJ.matWorld._14;
	m_ConstantLigth.m_Pos.y = m_ConstantOBJ.matWorld._24;
	m_ConstantLigth.m_Pos.z = m_ConstantOBJ.matWorld._34;

	D3DXMATRIX matInvWorld;
	D3DXMATRIX matWorld = GetConstantOBJ().matWorld;
	D3DXMatrixInverse(&matInvWorld, NULL, &matWorld);
	D3DXMatrixTranspose(&matInvWorld, &matInvWorld);
	D3DXMatrixTranspose(&m_ConstantLigth.m_matInvWorld, &matInvWorld);

	g_pImmediateContext->UpdateSubresource(m_pLightConstantBuffer, 0, 0, &m_ConstantLigth, 0, 0);
	return true;
}

bool MLight::Render()
{
	g_pImmediateContext->VSSetConstantBuffers(2, 1, &m_pLightConstantBuffer);
	g_pImmediateContext->PSSetConstantBuffers(2, 1, &m_pLightConstantBuffer);
	return true;
}

MLight::MLight()
{
}


MLight::~MLight()
{
}
