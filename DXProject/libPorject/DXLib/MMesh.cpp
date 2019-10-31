#include "MMesh.h"



MMesh::MMesh()
{
	m_VertexShaderID = VS3D;
}


MMesh::~MMesh()
{
}

bool MMesh::Init()
{
	return true;
}

bool MMesh::Frame()
{
	return true;
}

bool MMesh::Render()
{
	g_pImmediateContext->IASetInputLayout(I_VertexShaderMgr.m_LOList[m_VertexShaderID]);		// 레이아웃 셋
	g_pImmediateContext->VSSetShader(I_VertexShaderMgr.m_VSList[m_VertexShaderID], NULL, 0);
	UINT stride = sizeof(MVERTEX);		//// 보폭(카운트할 사이즈 크기)
	UINT offset = 0;
	g_pImmediateContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);
	g_pImmediateContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	g_pImmediateContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	g_pImmediateContext->DrawIndexed(m_iIndexCount, 0, 0);
	return true;
}

bool MMesh::Release()
{
	//VertexData.clear();
	//IndexData.clear();
	SAFE_RELEASE(m_pVertexBuffer);
	SAFE_RELEASE(m_pIndexBuffer);
	return true;
}