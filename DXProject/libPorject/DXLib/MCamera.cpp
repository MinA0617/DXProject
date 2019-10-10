#include "MCamera.h"



MCamera::MCamera()
{
	isRotition = true;
	yew = 0;
	pitch = 0;
}


MCamera::~MCamera()
{
}

bool MCamera::Init()
{
	D3DXMatrixIdentity(&m_matWorld);
	//// 카메라의 기본값 ////
	m_LookAt = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Up = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	D3DXMatrixLookAtLH(&m_matView, &GetPosition(), &m_LookAt, &m_Up);

	//// FOV 설정 ////
	D3DXMatrixPerspectiveFovLH(&m_matProj, D3DX_PI / 4, (float)g_rtWindowClient.right / (float)g_rtWindowClient.bottom, 1.0f, 10000.0f);

	D3DXMatrixTranspose(&m_matWorld, &m_matWorld);
	D3DXMatrixTranspose(&m_matView, &m_matView);
	D3DXMatrixTranspose(&m_matProj, &m_matProj);



	CreateConstantBuffer();
	return true;
}

bool MCamera::Frame()
{
	D3DXMatrixLookAtLH(&m_matView, &GetPosition(), &m_LookAt, &m_Up);
	D3DXMatrixTranspose(&m_matView, &m_matView);

	m_WorldPosition = m_LocalPosition;

	D3D11_MAPPED_SUBRESOURCE MappedResource;
	if (SUCCEEDED(g_pImmediateContext->Map(m_pConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedResource)))
	{
		VS_WORLD_BUFFER* data = (VS_WORLD_BUFFER*)MappedResource.pData;
		data->matView = m_matView;
		data->matProj = m_matProj;
		data->vEyePos = m_WorldPosition;
		data->vEyeDir = m_LookAt;
		data->fClinetSizeW = g_rtWindowClient.right;
		data->fClinetSizeH = g_rtWindowClient.bottom;
		g_pImmediateContext->Unmap(m_pConstantBuffer, 0);
	}
	return true;
}



bool MCamera::CreateConstantBuffer()
{
	//// 버퍼를 생성하고 디폴트 데이터를 넣어둔다 ////
	D3D11_BUFFER_DESC BufferDesc;
	HRESULT CreateBufferResult;

	VS_WORLD_BUFFER data;
	ZeroMemory(&data, sizeof(VS_WORLD_BUFFER));

	data.matView = m_matView;
	data.matProj = m_matProj;
	data.fClinetSizeW = g_rtWindowClient.right;
	data.fClinetSizeH = g_rtWindowClient.bottom;

	D3D11_SUBRESOURCE_DATA SubresourceData;
	ZeroMemory(&SubresourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	SubresourceData.pSysMem = &data;

	ZeroMemory(&BufferDesc, sizeof(D3D11_BUFFER_DESC));
	BufferDesc.ByteWidth = sizeof(VS_WORLD_BUFFER);
	BufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	BufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	BufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	if (FAILED(CreateBufferResult = g_pDevice->CreateBuffer(&BufferDesc, &SubresourceData, &m_pConstantBuffer))) return false;
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	return true;
}

bool MCamera::Render()
{
	g_pImmediateContext->VSSetConstantBuffers(0, 1, &m_pConstantBuffer);
	g_pImmediateContext->PSSetConstantBuffers(0, 1, &m_pConstantBuffer);
	return true;
}

bool MCamera::Release()
{
	return true;
}

///////////////////////////////////////////
void MCamera::MoveFront(float valve)
{
	m_LocalPosition += m_LookAt * valve * g_fSeoundPerFrame;
}

void MCamera::MoveBack(float valve)
{
	m_LocalPosition += m_LookAt * -valve * g_fSeoundPerFrame;
}

void MCamera::MoveRight(float valve)
{
	D3DXVECTOR3 Right;
	D3DXVec3Cross(&Right, &m_Up, &m_LookAt);
	m_LocalPosition += Right * valve * g_fSeoundPerFrame;
}

void MCamera::MoveLeft(float valve)
{
	D3DXVECTOR3 Right;
	D3DXVec3Cross(&Right, &m_Up, &m_LookAt);
	m_LocalPosition -= Right * valve * g_fSeoundPerFrame;
}