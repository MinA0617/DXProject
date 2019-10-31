#include "M2DScreen.h"



bool M2DScreen::Render()
{
	I_MaterialMgr[MaterialID]->Render();
	g_pImmediateContext->IASetInputLayout(I_VertexShaderMgr.m_LOList[m_VertexShaderID]);		// 레이아웃 셋
	g_pImmediateContext->VSSetShader(I_VertexShaderMgr.m_VSList[m_VertexShaderID], NULL, 0);
	UINT stride = sizeof(SVERTEX);		//// 보폭(카운트할 사이즈 크기)
	UINT offset = 0;					//// 시작 위치
	g_pImmediateContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);
	g_pImmediateContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	g_pImmediateContext->VSSetConstantBuffers(1, 1, &m_pConstantBuffer);
	/////////////////////////////////////////////////////////////////////////////////////
	g_pImmediateContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	g_pImmediateContext->DrawIndexed(6, 0, 0);
	return true;
}

M2DScreen::M2DScreen()
{
}


M2DScreen::~M2DScreen()
{
}
