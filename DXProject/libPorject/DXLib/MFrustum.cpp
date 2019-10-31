#include "MFrustum.h"

bool MPlane::CreatePlane(D3DXVECTOR3 v0, D3DXVECTOR3 v1, D3DXVECTOR3 v2)
{
	D3DXVECTOR3 vEdge0 = v1 - v0;
	D3DXVECTOR3 vEdge1 = v2 - v0;
	D3DXVECTOR3 vNormal;
	D3DXVec3Cross(&vNormal, &vEdge0, &vEdge1);
	D3DXVec3Normalize(&vNormal, &vNormal);
	fA = vNormal.x;
	fB = vNormal.y;
	fC = vNormal.z;
	fD = -(fA*v0.x + fB * v0.y + fC * v0.z);
	return true;
}

bool MPlane::CreatePlane(D3DXVECTOR3 vNormal, D3DXVECTOR3 v0)
{
	D3DXVec3Normalize(&vNormal, &vNormal);
	fA = vNormal.x;
	fB = vNormal.y;
	fC = vNormal.z;
	fD = -(fA*v0.x + fB * v0.y + fC * v0.z);
	return true;
}

void MPlane::Normalize()
{
	float fMag = sqrtf(fA*fA + fB * fB + fC * fC);
	fA = fA / fMag;
	fB = fB / fMag;
	fC = fC / fMag;
	fD = fD / fMag;
}

MFrustum::MFrustum()
{
}


MFrustum::~MFrustum()
{
}

bool MFrustum::CheckPlane(MPlane plane, MBoundingBox* box)
{
	float distance;
	D3DXVECTOR3 normal = D3DXVECTOR3(plane.fA, plane.fB, plane.fC);
	D3DXVECTOR3 dir0 = box->vAxis[0] * box->fOldExtent[0];
	D3DXVECTOR3 dir1 = box->vAxis[1] * box->fOldExtent[1];
	D3DXVECTOR3 dir2 = box->vAxis[2] * box->fOldExtent[2];
	distance = fabs(D3DXVec3Dot(&normal, &dir0));
	distance += fabs(D3DXVec3Dot(&normal, &dir1));
	distance += fabs(D3DXVec3Dot(&normal, &dir2));
	float fPlaneToCenter = plane.fA * box->vCenter.x + plane.fB * box->vCenter.y + plane.fC * box->vCenter.z + plane.fD;
	if (fPlaneToCenter <= -distance)
	{
		return false;
	}
	return true;
}

bool MFrustum::CheckOBB(MBoundingBox* box)
{
	if (box == nullptr) return true;
	for (int i = 0; i < 5; i++)
	{
		if (CheckPlane(m_Plane[i], box) == false)
		{
			return false;
		}
	}
	return true;
}

void MFrustum::CreateFrustum(D3DXMATRIX& matView, D3DXMATRIX& matProj)
{
	m_matView = matView;
	m_matProj = matProj;
	D3DXMatrixTranspose(&m_matView, &m_matView);
	D3DXMatrixTranspose(&m_matProj, &m_matProj);
	D3DXMATRIX matInvVP = m_matView * m_matProj;
	D3DXMatrixInverse(&matInvVP, NULL, &matInvVP);

	// 1   2
	// 4   3
	//   0
	m_vFrustum[0] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	m_vFrustum[1] = D3DXVECTOR3(-1.0f, 1.0f, 1.0f);
	m_vFrustum[2] = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	m_vFrustum[3] = D3DXVECTOR3(1.0f, -1.0f, 1.0f);
	m_vFrustum[4] = D3DXVECTOR3(-1.0f, -1.0f, 1.0f);

	m_vFrustum[1] = D3DXVECTOR3(-1.0f, 1.0f, 0.999f);
	m_vFrustum[2] = D3DXVECTOR3(1.0f, 1.0f, 0.999f);
	m_vFrustum[3] = D3DXVECTOR3(1.0f, -1.0f, 0.999f);
	m_vFrustum[4] = D3DXVECTOR3(-1.0f, -1.0f, 0.999f);

	for (int i = 0; i < 5; i++)
	{
		D3DXVec3TransformCoord(&m_vFrustum[i], &m_vFrustum[i], &matInvVP);
	}

	m_Plane[0].CreatePlane(m_vFrustum[1], m_vFrustum[4], m_vFrustum[0]);	// 좌 평면(left)
	m_Plane[1].CreatePlane(m_vFrustum[3], m_vFrustum[2], m_vFrustum[0]);	// 우 평면(right)
	m_Plane[2].CreatePlane(m_vFrustum[2], m_vFrustum[1], m_vFrustum[0]);	// 상 평면(top)
	m_Plane[3].CreatePlane(m_vFrustum[4], m_vFrustum[3], m_vFrustum[0]);	// 하 평면(bottom)
	m_Plane[4].CreatePlane(m_vFrustum[2], m_vFrustum[3], m_vFrustum[1]);	// 원 평면(far)
}
