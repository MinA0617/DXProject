#include "MBoundingBox.h"
#include "PixelShaderMgr.h"
#include "VertexShaderMgr.h"
#include "MDxState.h"
#include "M3DObject.h"


bool MBoundingBox::Copy(MBoundingBox* target)
{
	if (target == nullptr) return false;
	n_bCanOverlap = target->n_bCanOverlap;
	vMin = target->vMin;
	vMax = target->vMax;
	vCenter = target->vCenter;
	vOldCenter = target->vOldCenter;
	vAxis[0] = target->vAxis[0];
	vAxis[1] = target->vAxis[1];
	vAxis[2] = target->vAxis[2];
	fOldExtent[0] = target->fOldExtent[0];
	fOldExtent[1] = target->fOldExtent[1];
	fOldExtent[2] = target->fOldExtent[2];
	fExtent[0] = target->fExtent[0];
	fExtent[1] = target->fExtent[1];
	fExtent[2] = target->fExtent[2];
	return true;
}

bool MBoundingBox::BuildForMinMax()
{
	vOldCenter = (vMax + vMin) / 2;
	fOldExtent[0] = vMax[0] - vMin[0];
	fOldExtent[1] = vMax[1] - vMin[1];
	fOldExtent[2] = vMax[2] - vMin[2];
	return true;
}

bool MBoundingBox::SelfUpdate()
{
	D3DXMATRIX mat;
	D3DXVECTOR3 scl(fExtent[0], fExtent[1], fExtent[2]);
	D3DXMatrixTransformation(&mat, NULL, NULL, &scl, NULL, NULL, &vCenter);
	vPoint[0] = D3DXVECTOR3(-1, -1, -1);
	vPoint[1] = D3DXVECTOR3(-1, -1, 1);
	vPoint[2] = D3DXVECTOR3(-1, 1, -1);
	vPoint[3] = D3DXVECTOR3(-1, 1, 1);
	vPoint[4] = D3DXVECTOR3(1, -1, -1);
	vPoint[5] = D3DXVECTOR3(1, -1, 1);
	vPoint[6] = D3DXVECTOR3(1, 1, -1);
	vPoint[7] = D3DXVECTOR3(1, 1, 1);
	for (int i = 0; i < 8; i++)
	{
		D3DXVec3TransformCoord(&vPoint[i], &vPoint[i], &mat);
	}
#if defined(DEBUG) || defined(_DEBUG)
	if (m_pConstantBuffer)
	{
		D3DXMatrixTranspose(&mat, &mat);
		g_pImmediateContext->UpdateSubresource(m_pConstantBuffer, 0, 0, &mat, 0, 0);
	}
#endif
	return true;
}

//bool MBoundingBox::Updata(D3DXMATRIX& mat)
bool MBoundingBox::Updata(D3DXVECTOR3* pos, D3DXQUATERNION* rot, D3DXVECTOR3* scl)
{
	D3DXMATRIX matWorld;
	D3DXVECTOR3 scale = *scl;
	D3DXMatrixTransformation(&matWorld, NULL, NULL, scl, NULL, rot, pos);
	D3DXVec3TransformCoord(&vCenter, &vOldCenter, &matWorld);
	fExtent[0] = fOldExtent[0] * scl->x;
	fExtent[1] = fOldExtent[1] * scl->y;
	fExtent[2] = fOldExtent[2] * scl->z;
	vMin.x = vCenter.x - fExtent[0];
	vMin.y = vCenter.y - fExtent[1];
	vMin.z = vCenter.z - fExtent[2];
	vMax.x = vCenter.x + fExtent[0];
	vMax.y = vCenter.y + fExtent[1];
	vMax.z = vCenter.z + fExtent[2];
	D3DXVECTOR3 vOldAxis[3];
	vOldAxis[0] = D3DXVECTOR3(1, 0, 0);
	vOldAxis[1] = D3DXVECTOR3(0, 1, 0);
	vOldAxis[2] = D3DXVECTOR3(0, 0, 1);
	matWorld._41 = 0;
	matWorld._42 = 0;
	matWorld._43 = 0;
	D3DXVec3TransformCoord(&vAxis[0], &vOldAxis[0], &matWorld);
	D3DXVec3TransformCoord(&vAxis[1], &vOldAxis[1], &matWorld);
	D3DXVec3TransformCoord(&vAxis[2], &vOldAxis[2], &matWorld);
	D3DXVec3Normalize(&vAxis[0], &vAxis[0]);
	D3DXVec3Normalize(&vAxis[1], &vAxis[1]);
	D3DXVec3Normalize(&vAxis[2], &vAxis[2]);

	scale.x = fExtent[0];
	scale.y = fExtent[1];
	scale.z = fExtent[2];
	D3DXMatrixTransformation(&matWorld, NULL, NULL, &scale, NULL, rot, &vCenter);
	vPoint[0] = D3DXVECTOR3(-1, -1, -1);
	vPoint[1] = D3DXVECTOR3(-1, -1, 1);
	vPoint[2] = D3DXVECTOR3(-1, 1, -1);
	vPoint[3] = D3DXVECTOR3(-1, 1, 1);
	vPoint[4] = D3DXVECTOR3(1, -1, -1);
	vPoint[5] = D3DXVECTOR3(1, -1, 1);
	vPoint[6] = D3DXVECTOR3(1, 1, -1);
	vPoint[7] = D3DXVECTOR3(1, 1, 1);
	for (int i = 0; i < 8; i++)
	{
		D3DXVec3TransformCoord(&vPoint[i], &vPoint[i], &matWorld);
	}
#if defined(DEBUG) || defined(_DEBUG)
	if (m_pConstantBuffer)
	{
		D3DXMatrixTranspose(&matWorld, &matWorld);
		g_pImmediateContext->UpdateSubresource(m_pConstantBuffer, 0, 0, &matWorld, 0, 0);
	}
#endif

	return true;
}

bool MBoundingBox::UpdataMaxY(float maxy)
{
	// 노드한정 //
	vMax.y = maxy;
	vCenter.y = vMax.y / 2;
	vOldCenter.y = vMax.y / 2;
	fExtent[1] = vMax.y / 2;
	fOldExtent[1] = vMax.y / 2;
	SelfUpdate();
	return true;
}

#if defined(DEBUG) || defined(_DEBUG)
bool MBoundingBox::Init()
{
	D3D11_BUFFER_DESC BufferDesc;				//// 공용 버퍼 데스크
	HRESULT CreateBufferResult;					//// 공용 결과값

	ZeroMemory(&BufferDesc, sizeof(D3D11_BUFFER_DESC));
	BufferDesc.ByteWidth = sizeof(CONSTANT_3DOBJ);
	BufferDesc.Usage = D3D11_USAGE_DEFAULT;
	BufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

	if (FAILED(CreateBufferResult = g_pDevice->CreateBuffer(&BufferDesc, NULL, &m_pConstantBuffer))) return false;
	return true;
}

bool MBoundingBox::Render()
{
	if (m_pConstantBuffer == NULL)
	{
		Init();
	}
	I_DxState.BS_Set(MAlphaBlend);
	I_DxState.SS_Set(MWrapLinear);
	I_DxState.DSS_Set(MDepthEnable);

	//m_pImmediateContext->HSSetShader(m_pHullShader, NULL, 0);
	//m_pImmediateContext->DSSetShader(m_pDomainShader, NULL, 0);
	//m_pImmediateContext->GSSetShader(m_pGeometryShader, NULL, 0);
	g_pImmediateContext->PSSetShader(I_PixelShaderMgr.m_PSList[PSSPLINE], NULL, 0);
	g_pImmediateContext->IASetInputLayout(I_VertexShaderMgr.m_LOList[VSSPLINE]);		// 레이아웃 셋
	g_pImmediateContext->VSSetShader(I_VertexShaderMgr.m_VSList[VSSPLINE], NULL, 0);
	UINT stride = sizeof(SPLINE_VERTEX);		//// 보폭(카운트할 사이즈 크기)
	UINT offset = 0;					//// 시작 위치
	g_pImmediateContext->IASetVertexBuffers(0, 1, &g_pVB_BoundingBox, &stride, &offset);
	g_pImmediateContext->IASetIndexBuffer(g_pIB_BoundingBox, DXGI_FORMAT_R32_UINT, 0);
	g_pImmediateContext->VSSetConstantBuffers(1, 1, &m_pConstantBuffer);

	/////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////
	g_pImmediateContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINELIST);
	//g_pImmediateContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_POINTLIST);
	g_pImmediateContext->DrawIndexed(g_dwIC_BoundingBox, 0, 0);
	return true;
}

bool MBoundingBox::Release()
{
	SAFE_RELEASE(m_pConstantBuffer);
	return true;
}

#endif
MBoundingBox::MBoundingBox()
{
	m_pConstantBuffer = NULL;
	vMin = D3DXVECTOR3(0, 0, 0);
	vMax = D3DXVECTOR3(0, 0, 0);
	vCenter = D3DXVECTOR3(0, 0, 0);
	vAxis[0] = D3DXVECTOR3(1, 0, 0);
	vAxis[1] = D3DXVECTOR3(0, 1, 0);
	vAxis[2] = D3DXVECTOR3(0, 0, 1);
	fOldExtent[0] = 0;
	fOldExtent[1] = 0;
	fOldExtent[2] = 0;
	n_bCanOverlap = false;
}


MBoundingBox::~MBoundingBox()
{
}
