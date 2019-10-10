#include "VertexShaderMgr.h"



VertexShaderMgr::VertexShaderMgr()
{
	for (int i = 0; i < VS_NUM; i++)
	{
		m_VSList[i] = NULL;
	}
}


VertexShaderMgr::~VertexShaderMgr()
{
}

bool VertexShaderMgr::Init()
{
	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined(DEBUG) || defined(_DEBUG)
	dwShaderFlags |= D3DCOMPILE_DEBUG;
#endif
	HRESULT LoadShaderResult;
	ID3D10Blob* pVSShader;
	ID3D10Blob* pErrorMsgs;
#pragma region VS2D
	LoadShaderResult = D3DX11CompileFromFile(L"../../data/Shader/VS2D.vsh", NULL, NULL, "VS", "vs_5_0", 0, 0, NULL, &pVSShader, &pErrorMsgs, NULL);
	if (FAILED(LoadShaderResult))
	{
		MessageBoxA(g_hWnd, (char*)pErrorMsgs->GetBufferPointer(), "Error", MB_OK);
		return false;
	}
	g_pDevice->CreateVertexShader(pVSShader->GetBufferPointer(), pVSShader->GetBufferSize(), NULL, &m_VSList[VS2D]);	// 컴파일된 쉐이더를 생성해 준다
	const D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{"POSITION"	, 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{"TEXCOORD"	, 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	int layoutNum = _countof(layout);
	LoadShaderResult = g_pDevice->CreateInputLayout(layout, layoutNum, pVSShader->GetBufferPointer(), pVSShader->GetBufferSize(), &m_LOList[VS2D]);
	if (pVSShader)pVSShader->Release();
	if (pErrorMsgs)pErrorMsgs->Release();
	if (FAILED(LoadShaderResult)) return false;
#pragma endregion VS2D

#pragma region VS3D
	LoadShaderResult = D3DX11CompileFromFile(L"../../data/Shader/VS3D.vsh", NULL, NULL, "VS", "vs_5_0", dwShaderFlags, 0, NULL, &pVSShader, &pErrorMsgs, NULL);
	if (FAILED(LoadShaderResult))
	{
		MessageBoxA(g_hWnd, (char*)pErrorMsgs->GetBufferPointer(), "Error", MB_OK);
		return false;
	}
	g_pDevice->CreateVertexShader(pVSShader->GetBufferPointer(), pVSShader->GetBufferSize(), NULL, &m_VSList[VS3D]);	// 컴파일된 쉐이더를 생성해 준다
	const D3D11_INPUT_ELEMENT_DESC layout2[] =
	{
		{"POSITION",		0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{"TEXCOORD",		0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{"NORMAL",			0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{"TANGENTVECTOR",	0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 36, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	layoutNum = _countof(layout2);
	LoadShaderResult = g_pDevice->CreateInputLayout(layout2, layoutNum, pVSShader->GetBufferPointer(), pVSShader->GetBufferSize(), &m_LOList[VS3D]);
	if (pVSShader)pVSShader->Release();
	if (pErrorMsgs)pErrorMsgs->Release();
	if (FAILED(LoadShaderResult)) return false;
#pragma endregion VS3D

#pragma region VSSPLINE
	LoadShaderResult = D3DX11CompileFromFile(L"../../data/Shader/VSSPLINE.vsh", NULL, NULL, "VS", "vs_5_0", dwShaderFlags, 0, NULL, &pVSShader, &pErrorMsgs, NULL);
	if (FAILED(LoadShaderResult))
	{
		MessageBoxA(g_hWnd, (char*)pErrorMsgs->GetBufferPointer(), "Error", MB_OK);
		return false;
	}
	g_pDevice->CreateVertexShader(pVSShader->GetBufferPointer(), pVSShader->GetBufferSize(), NULL, &m_VSList[VSSPLINE]);	// 컴파일된 쉐이더를 생성해 준다
	const D3D11_INPUT_ELEMENT_DESC layout3[] =
	{
		{"POSITION"	, 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{"NORMAL"	, 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{"TYPE"		, 0, DXGI_FORMAT_R32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	layoutNum = _countof(layout3);
	LoadShaderResult = g_pDevice->CreateInputLayout(layout3, layoutNum, pVSShader->GetBufferPointer(), pVSShader->GetBufferSize(), &m_LOList[VSSPLINE]);
	if (pVSShader)pVSShader->Release();
	if (pErrorMsgs)pErrorMsgs->Release();
	if (FAILED(LoadShaderResult)) return false;
#pragma endregion VSSPLINE
#pragma region VSCHARACTER
	LoadShaderResult = D3DX11CompileFromFile(L"../../data/Shader/VSCHARACTER.vsh", NULL, NULL, "VS", "vs_5_0", dwShaderFlags, 0, NULL, &pVSShader, &pErrorMsgs, NULL);
	if (FAILED(LoadShaderResult))
	{
		MessageBoxA(g_hWnd, (char*)pErrorMsgs->GetBufferPointer(), "Error", MB_OK);
		return false;
	}
	g_pDevice->CreateVertexShader(pVSShader->GetBufferPointer(), pVSShader->GetBufferSize(), NULL, &m_VSList[VSCHARACTER]);	// 컴파일된 쉐이더를 생성해 준다
	const D3D11_INPUT_ELEMENT_DESC layout4[] =
	{
		{"POSITION",		0, DXGI_FORMAT_R32G32B32_FLOAT,		0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{"TEXCOORD",		0, DXGI_FORMAT_R32G32B32_FLOAT,		0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{"NORMAL",			0, DXGI_FORMAT_R32G32B32_FLOAT,		0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{"TANGENTVECTOR",	0, DXGI_FORMAT_R32G32B32_FLOAT,		0, 36, D3D11_INPUT_PER_VERTEX_DATA, 0 },

		{"NUMWEIGHT",		0, DXGI_FORMAT_R32_SINT,			1, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{"BONEID",			0, DXGI_FORMAT_R32G32B32A32_SINT,	1, 4, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{"BONEWEIGHT",		0, DXGI_FORMAT_R32G32B32A32_FLOAT,	1, 20, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	layoutNum = _countof(layout4);
	LoadShaderResult = g_pDevice->CreateInputLayout(layout4, layoutNum, pVSShader->GetBufferPointer(), pVSShader->GetBufferSize(), &m_LOList[VSCHARACTER]);
	if (pVSShader)pVSShader->Release();
	if (pErrorMsgs)pErrorMsgs->Release();
	if (FAILED(LoadShaderResult)) return false;
#pragma endregion VSCHARACTER

	return true;
}

bool VertexShaderMgr::Release()
{
	for (int i = 0; i < VS_NUM; i++)
	{
		if (m_VSList[i] != NULL)
		{
			m_VSList[i]->Release();
		}
	}
	return true;
}