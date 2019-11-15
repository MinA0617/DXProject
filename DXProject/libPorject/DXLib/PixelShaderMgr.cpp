#include "PixelShaderMgr.h"



PixelShaderMgr::PixelShaderMgr()
{
	for (int i = 0; i < PS_NUM; i++)
	{
		m_PSList[i] = NULL;
	}
}


PixelShaderMgr::~PixelShaderMgr()
{
}

bool PixelShaderMgr::Init()
{
	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined(DEBUG) || defined(_DEBUG)
	dwShaderFlags |= D3DCOMPILE_DEBUG;
#endif

	HRESULT LoadShaderResult;
	ID3D10Blob* pPSShader;
	ID3D10Blob* pErrorMsgs;


#pragma region PS2D
	LoadShaderResult = D3DX11CompileFromFile(L"../../data/Shader/PS2D.psh", NULL, NULL, "PS", "ps_5_0", dwShaderFlags, 0, NULL, &pPSShader, &pErrorMsgs, NULL);
	if (FAILED(LoadShaderResult))
	{
		MessageBoxA(g_hWnd, (char*)pErrorMsgs->GetBufferPointer(), "Error", MB_OK);
		return false;
	}
	g_pDevice->CreatePixelShader(pPSShader->GetBufferPointer(), pPSShader->GetBufferSize(), NULL, &m_PSList[PS2D]);	// �����ϵ� ���̴��� ������ �ش�
#pragma endregion PS2D

#pragma region PS3D
	LoadShaderResult = D3DX11CompileFromFile(L"../../data/Shader/PS3D.psh", NULL, NULL, "PS", "ps_5_0", dwShaderFlags, 0, NULL, &pPSShader, &pErrorMsgs, NULL);
	if (FAILED(LoadShaderResult))
	{
		MessageBoxA(g_hWnd, (char*)pErrorMsgs->GetBufferPointer(), "Error", MB_OK);
		return false;
	}
	g_pDevice->CreatePixelShader(pPSShader->GetBufferPointer(), pPSShader->GetBufferSize(), NULL, &m_PSList[PS3D]);	// �����ϵ� ���̴��� ������ �ش�
#pragma endregion PS3D

#pragma region PSSPLINE
	LoadShaderResult = D3DX11CompileFromFile(L"../../data/Shader/PSSPLINE.psh", NULL, NULL, "PS", "ps_5_0", dwShaderFlags, 0, NULL, &pPSShader, &pErrorMsgs, NULL);
	if (FAILED(LoadShaderResult))
	{
		MessageBoxA(g_hWnd, (char*)pErrorMsgs->GetBufferPointer(), "Error", MB_OK);
		return false;
	}
	g_pDevice->CreatePixelShader(pPSShader->GetBufferPointer(), pPSShader->GetBufferSize(), NULL, &m_PSList[PSSPLINE]);	// �����ϵ� ���̴��� ������ �ش�
#pragma endregion PSSPLINE

#pragma region PSBONE
		LoadShaderResult = D3DX11CompileFromFile(L"../../data/Shader/PSBONE.psh", NULL, NULL, "PS", "ps_5_0", dwShaderFlags, 0, NULL, &pPSShader, &pErrorMsgs, NULL);
	if (FAILED(LoadShaderResult))
	{
		MessageBoxA(g_hWnd, (char*)pErrorMsgs->GetBufferPointer(), "Error", MB_OK);
		return false;
	}
	g_pDevice->CreatePixelShader(pPSShader->GetBufferPointer(), pPSShader->GetBufferSize(), NULL, &m_PSList[PSBONE]);	// �����ϵ� ���̴��� ������ �ش�
#pragma endregion PSBONE

#pragma region PSFILED
	LoadShaderResult = D3DX11CompileFromFile(L"../../data/Shader/PS3DFiled.psh", NULL, NULL, "PS", "ps_5_0", dwShaderFlags, 0, NULL, &pPSShader, &pErrorMsgs, NULL);
	if (FAILED(LoadShaderResult))
	{
		MessageBoxA(g_hWnd, (char*)pErrorMsgs->GetBufferPointer(), "Error", MB_OK);
		return false;
	}
	g_pDevice->CreatePixelShader(pPSShader->GetBufferPointer(), pPSShader->GetBufferSize(), NULL, &m_PSList[PSFILED]);	// �����ϵ� ���̴��� ������ �ش�
#pragma endregion PSFILED

//#pragma region PSFILED2
//	LoadShaderResult = D3DX11CompileFromFile(L"../../data/Shader/PS3DFiled2.psh", NULL, NULL, "PS", "ps_5_0", dwShaderFlags, 0, NULL, &pPSShader, &pErrorMsgs, NULL);
//	if (FAILED(LoadShaderResult))
//	{
//		MessageBoxA(g_hWnd, (char*)pErrorMsgs->GetBufferPointer(), "Error", MB_OK);
//		return false;
//	}
//	g_pDevice->CreatePixelShader(pPSShader->GetBufferPointer(), pPSShader->GetBufferSize(), NULL, &m_PSList[PSFILED2]);	// �����ϵ� ���̴��� ������ �ش�
//#pragma endregion PSFILED2

	return true;
}

bool PixelShaderMgr::Release()
{
	for (int i = 0; i < PS_NUM; i++)
	{
		if (m_PSList[i] != NULL)
		{
			m_PSList[i]->Release();
		}
	}
	return true;
}