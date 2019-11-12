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

//bool MBoundingBox::Updata(D3DXMATRIX& mat)
bool MBoundingBox::Updata()
{
	if (m_pTarget == nullptr) return false;
	D3DXVECTOR3* pos = &m_pTarget->m_WorldPosition;
	D3DXQUATERNION* rot = &m_pTarget->m_WorldRotation;
	D3DXVECTOR3* scl = &m_pTarget->m_WorldScale;
	D3DXMATRIX matWorld;
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

#if defined(DEBUG) || defined(_DEBUG)
	if (m_pConstantBuffer)
	{
		scl->x = fExtent[0];
		scl->y = fExtent[1];
		scl->z = fExtent[2];
		D3DXMatrixTransformation(&matWorld, NULL, NULL, scl, NULL, rot, &vCenter);
		D3DXMatrixTranspose(&matWorld, &matWorld);
		g_pImmediateContext->UpdateSubresource(m_pConstantBuffer, 0, 0, &matWorld, 0, 0);
	}
#endif

	return false;
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
	m_pTarget = nullptr;
}


MBoundingBox::~MBoundingBox()
{
}
