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
//#if defined(DEBUG) || defined(_DEBUG)
//	dwShaderFlags |= D3DCOMPILE_DEBUG;
//#endif
	HRESULT LoadShaderResult;
	ID3D10Blob* pVSShader;
	ID3D10Blob* pErrorMsgs;
#pragma region VS2D
	LoadShaderResult = D3DX11CompileFromFile(L"../../data/Shader/VS2D.vsh", NULL, NULL, "VS", "vs_5_0", dwShaderFlags, 0, NULL, &pVSShader, &pErrorMsgs, NULL);
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


#pragma region VSFILED
	LoadShaderResult = D3DX11CompileFromFile(L"../../data/Shader/VS3DFiled.vsh", NULL, NULL, "VS", "vs_5_0", dwShaderFlags, 0, NULL, &pVSShader, &pErrorMsgs, NULL);
	if (FAILED(LoadShaderResult))
	{
		MessageBoxA(g_hWnd, (char*)pErrorMsgs->GetBufferPointer(), "Error", MB_OK);
		return false;
	}
	g_pDevice->CreateVertexShader(pVSShader->GetBufferPointer(), pVSShader->GetBufferSize(), NULL, &m_VSList[VSFILED]);	// 컴파일된 쉐이더를 생성해 준다
	const D3D11_INPUT_ELEMENT_DESC layout5[] =
	{
		{"POSITION",		0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{"TEXCOORD",		0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{"NORMAL",			0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{"TANGENTVECTOR",	0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 36, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	layoutNum = _countof(layout5);
	LoadShaderResult = g_pDevice->CreateInputLayout(layout5, layoutNum, pVSShader->GetBufferPointer(), pVSShader->GetBufferSize(), &m_LOList[VSFILED]);
	if (pVSShader)pVSShader->Release();
	if (pErrorMsgs)pErrorMsgs->Release();
	if (FAILED(LoadShaderResult)) return false;
#pragma endregion VS3DINSTANCE

#pragma region VS3DINSTANCE
	LoadShaderResult = D3DX11CompileFromFile(L"../../data/Shader/3DINSTANCE.hlsl", NULL, NULL, "VS", "vs_5_0", dwShaderFlags, 0, NULL, &pVSShader, &pErrorMsgs, NULL);
	if (FAILED(LoadShaderResult))
	{
		MessageBoxA(g_hWnd, (char*)pErrorMsgs->GetBufferPointer(), "Error", MB_OK);
		return false;
	}
	g_pDevice->CreateVertexShader(pVSShader->GetBufferPointer(), pVSShader->GetBufferSize(), NULL, &m_VSList[VS3DINSTANCE]);	// 컴파일된 쉐이더를 생성해 준다
	const D3D11_INPUT_ELEMENT_DESC layout6[] =
	{
		{"POSITION",		0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{"TEXCOORD",		0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{"NORMAL",			0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{"TANGENTVECTOR",	0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 36, D3D11_INPUT_PER_VERTEX_DATA, 0 },

		{"WORLDMAT",		0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 0, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{"WORLDMAT",		1, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{"WORLDMAT",		2, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 32, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{"WORLDMAT",		3, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 48, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
	};
	layoutNum = _countof(layout6);
	LoadShaderResult = g_pDevice->CreateInputLayout(layout6, layoutNum, pVSShader->GetBufferPointer(), pVSShader->GetBufferSize(), &m_LOList[VS3DINSTANCE]);
	if (pVSShader)pVSShader->Release();
	if (pErrorMsgs)pErrorMsgs->Release();
	if (FAILED(LoadShaderResult)) return false;
#pragma endregion VS3DINSTANCE
	
#pragma region VS3DSKININSTANCE
	LoadShaderResult = D3DX11CompileFromFile(L"../../data/Shader/3DINSTANCESKIN.hlsl", NULL, NULL, "VS", "vs_5_0", dwShaderFlags, 0, NULL, &pVSShader, &pErrorMsgs, NULL);
	if (FAILED(LoadShaderResult))
	{
		MessageBoxA(g_hWnd, (char*)pErrorMsgs->GetBufferPointer(), "Error", MB_OK);
		return false;
	}
	g_pDevice->CreateVertexShader(pVSShader->GetBufferPointer(), pVSShader->GetBufferSize(), NULL, &m_VSList[VS3DSKININSTANCE]);	// 컴파일된 쉐이더를 생성해 준다
	vector<D3D11_INPUT_ELEMENT_DESC> layoutvec;
	layoutvec.reserve(267);
	const D3D11_INPUT_ELEMENT_DESC layout7[] =
	{
		{"POSITION",		0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{"TEXCOORD",		0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{"NORMAL",			0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{"TANGENTVECTOR",	0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 36, D3D11_INPUT_PER_VERTEX_DATA, 0 },

		{"NUMWEIGHT",		0, DXGI_FORMAT_R32_SINT,			1, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{"BONEID",			0, DXGI_FORMAT_R32G32B32A32_SINT,	1, 4, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{"BONEWEIGHT",		0, DXGI_FORMAT_R32G32B32A32_FLOAT,	1, 20, D3D11_INPUT_PER_VERTEX_DATA, 0 },

		{"WORLDMAT",		0, DXGI_FORMAT_R32G32B32A32_FLOAT, 2, 0, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{"WORLDMAT",		1, DXGI_FORMAT_R32G32B32A32_FLOAT, 2, 16, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{"WORLDMAT",		2, DXGI_FORMAT_R32G32B32A32_FLOAT, 2, 32, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{"WORLDMAT",		3, DXGI_FORMAT_R32G32B32A32_FLOAT, 2, 48, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
	};
	for (int i = 0; i < _countof(layout7); i++)
	{
		layoutvec.push_back(layout7[i]);
	}
	for (int i = 0; i < MAX_BONE; i++)
	{
		D3D11_INPUT_ELEMENT_DESC forlayout;
		forlayout.SemanticName = "BONEMAT";
		forlayout.SemanticIndex = i;
		forlayout.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		forlayout.InputSlot = 3;
		forlayout.AlignedByteOffset = i * 16;
		forlayout.InputSlotClass = D3D11_INPUT_PER_INSTANCE_DATA;
		forlayout.InstanceDataStepRate = 1;
		layoutvec.push_back(forlayout);
	}
	layoutNum = layoutvec.size();
	LoadShaderResult = g_pDevice->CreateInputLayout(&layoutvec.at(0), layoutNum, pVSShader->GetBufferPointer(), pVSShader->GetBufferSize(), &m_LOList[VS3DSKININSTANCE]);
	if (pVSShader)pVSShader->Release();
	if (pErrorMsgs)pErrorMsgs->Release();
	if (FAILED(LoadShaderResult)) return false;
#pragma endregion VS3DSKININSTANCE
//#pragma region VSFILED2
//	LoadShaderResult = D3DX11CompileFromFile(L"../../data/Shader/VS3DFiled2.vsh", NULL, NULL, "VS", "vs_5_0", 0, 0, NULL, &pVSShader, &pErrorMsgs, NULL);
//	if (FAILED(LoadShaderResult))
//	{
//		MessageBoxA(g_hWnd, (char*)pErrorMsgs->GetBufferPointer(), "Error", MB_OK);
//		return false;
//	}
//	g_pDevice->CreateVertexShader(pVSShader->GetBufferPointer(), pVSShader->GetBufferSize(), NULL, &m_VSList[VSFILED2]);	// 컴파일된 쉐이더를 생성해 준다
//	const D3D11_INPUT_ELEMENT_DESC layout6[] =
//	{
//		{"POSITION",		0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
//		{"COUNT",			0, DXGI_FORMAT_R32_SINT, 0, 8, D3D11_INPUT_PER_VERTEX_DATA, 0 },
//		{"TYPE",			0, DXGI_FORMAT_R32_SINT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
//	};
//	layoutNum = _countof(layout6);
//	LoadShaderResult = g_pDevice->CreateInputLayout(layout6, layoutNum, pVSShader->GetBufferPointer(), pVSShader->GetBufferSize(), &m_LOList[VSFILED2]);
//	if (pVSShader)pVSShader->Release();
//	if (pErrorMsgs)pErrorMsgs->Release();
//	if (FAILED(LoadShaderResult)) return false;
//#pragma endregion VSFILED2
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