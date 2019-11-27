#include "MCamera.h"


MCamera::MCamera()
{
	isRotition = false;
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

	D3DXMatrixLookAtLH(&m_matView, &GetLocalPosition(), &m_LookAt, &m_Up);

	//// FOV 설정 ////
	D3DXMatrixPerspectiveFovLH(&m_matProj, D3DX_PI / 4, (float)g_rtWindowClient.right / (float)g_rtWindowClient.bottom, 1.0f, MAXDISTANCE);

	D3DXMatrixTranspose(&m_matWorld, &m_matWorld);

	CreateConstantBuffer();
	return true;
}

bool MCamera::CameraFrame()
{
	m_WorldPosition = m_LocalPosition;
	m_WorldConstant.matViewProj = m_matView * m_matProj;
	D3DXMatrixTranspose(&m_WorldConstant.matViewProj, &m_WorldConstant.matViewProj);
	m_WorldConstant.vEyePos = m_WorldPosition;
	m_WorldConstant.vEyeDir = m_LookAt;
	m_WorldConstant.fClinetSizeW = g_rtWindowClient.right;
	m_WorldConstant.fClinetSizeH = g_rtWindowClient.bottom;
	if (m_pConstantBuffer)
	{
		g_pImmediateContext->UpdateSubresource(m_pConstantBuffer, 0, 0, &m_WorldConstant, 0, 0);
	}
	return false;
}

bool MCamera::Frame()
{
	D3DXMatrixLookAtLH(&m_matView, &GetLocalPosition(), &m_LookAt, &m_Up);
	CameraFrame();
	return true;
}



bool MCamera::CreateConstantBuffer()
{
	//// 버퍼를 생성하고 디폴트 데이터를 넣어둔다 ////
	D3D11_BUFFER_DESC BufferDesc;
	HRESULT CreateBufferResult;

	ZeroMemory(&m_WorldConstant, sizeof(VS_WORLD_BUFFER));

	m_WorldConstant.matViewProj = m_matView * m_matView;
	m_WorldConstant.fClinetSizeW = g_rtWindowClient.right;
	m_WorldConstant.fClinetSizeH = g_rtWindowClient.bottom;

	D3D11_SUBRESOURCE_DATA SubresourceData;
	ZeroMemory(&SubresourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	SubresourceData.pSysMem = &m_WorldConstant;

	ZeroMemory(&BufferDesc, sizeof(D3D11_BUFFER_DESC));
	BufferDesc.ByteWidth = sizeof(VS_WORLD_BUFFER);
	BufferDesc.Usage = D3D11_USAGE_DEFAULT;
	BufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	//BufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
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
	D3DXVec3Normalize(&Right, &Right);
	m_LocalPosition += Right * valve * g_fSeoundPerFrame;
}

void MCamera::MoveLeft(float valve)
{
	D3DXVECTOR3 Right;
	D3DXVec3Cross(&Right, &m_Up, &m_LookAt);
	D3DXVec3Normalize(&Right, &Right);
	m_LocalPosition -= Right * valve * g_fSeoundPerFrame;
}

void MCamera::Resize()
{
	D3DXMatrixPerspectiveFovLH(&m_matProj, D3DX_PI / 4, (float)g_rtWindowClient.right / (float)g_rtWindowClient.bottom, 1.0f, MAXDISTANCE);
}
