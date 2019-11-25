#include "MHM.h"



bool MHM::Init()
{
	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
	#if defined(DEBUG) || defined(_DEBUG)
		dwShaderFlags |= D3DCOMPILE_DEBUG;
	#endif
	HRESULT LoadShaderResult;
	ID3D10Blob* pBlob;
	ID3D10Blob* pErrorMsgs;

	LoadShaderResult = D3DX11CompileFromFile(L"../../data/Shader/test/VS3D.vsh", NULL, NULL, "VS", "vs_5_0", dwShaderFlags, 0, NULL, &pBlob, &pErrorMsgs, NULL);
	if (FAILED(LoadShaderResult))
	{
		MessageBoxA(g_hWnd, (char*)pErrorMsgs->GetBufferPointer(), "Error", MB_OK);
		return false;
	}
	g_pDevice->CreateVertexShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), NULL, &vs);	// 컴파일된 쉐이더를 생성해 준다
	const D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{"POSITION"	, 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{"WORLDPOS"	, 0, DXGI_FORMAT_R32G32_FLOAT, 1, 0, D3D11_INPUT_PER_INSTANCE_DATA, 0 },
		{"LEVEL"	, 0, DXGI_FORMAT_R32_FLOAT, 1, 8, D3D11_INPUT_PER_INSTANCE_DATA, 0 },
	};
	int layoutNum = _countof(layout);
	LoadShaderResult = g_pDevice->CreateInputLayout(layout, layoutNum, pBlob->GetBufferPointer(), pBlob->GetBufferSize(), &lo);
	if (pBlob)pBlob->Release();
	if (pErrorMsgs)pErrorMsgs->Release();
	if (FAILED(LoadShaderResult)) return false;
// ---------------------------------------------------------
	LoadShaderResult = D3DX11CompileFromFile(L"../../data/Shader/test/PS3D.psh", NULL, NULL, "PS", "ps_5_0", dwShaderFlags, 0, NULL, &pBlob, &pErrorMsgs, NULL);
	if (FAILED(LoadShaderResult))
	{
		MessageBoxA(g_hWnd, (char*)pErrorMsgs->GetBufferPointer(), "Error", MB_OK);
		return false;
	}
	g_pDevice->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), NULL, &ps);	// 컴파일된 쉐이더를 생성해 준다



	return true;
}

bool MHM::Render()
{
	g_pImmediateContext->HSSetShader(hs,)
	g_pImmediateContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_4_CONTROL_POINT_PATCHLIST);

	return true;
}

MHM::MHM()
{
}


MHM::~MHM()
{
}
