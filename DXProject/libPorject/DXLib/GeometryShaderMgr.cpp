//#include "GeometryShaderMgr.h"
//
//
//
//bool GeometryShaderMgr::Init()
//{
//	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
//#if defined(DEBUG) || defined(_DEBUG)
//	dwShaderFlags |= D3DCOMPILE_DEBUG;
//#endif
//
//	HRESULT LoadShaderResult;
//	ID3D10Blob* pGSShader;
//	ID3D10Blob* pErrorMsgs;
//
//#pragma region PSFILED2
//	LoadShaderResult = D3DX11CompileFromFile(L"../../data/Shader/GS3DFiled2.gsh", NULL, NULL, "GS", "gs_5_0", dwShaderFlags, 0, NULL, &pGSShader, &pErrorMsgs, NULL);
//	if (FAILED(LoadShaderResult))
//	{
//		MessageBoxA(g_hWnd, (char*)pErrorMsgs->GetBufferPointer(), "Error", MB_OK);
//		return false;
//	}
//	g_pDevice->CreateGeometryShader(pGSShader->GetBufferPointer(), pGSShader->GetBufferSize(), NULL, &m_GSList[GSFILED]);
//#pragma endregion PSFILED2
//	return true;
//}
//
//bool GeometryShaderMgr::Release()
//{
//	for (int i = 0; i < GS_NUM; i++)
//	{
//		SAFE_RELEASE(m_GSList[i]);
//	}
//	return true;
//}
//
//GeometryShaderMgr::GeometryShaderMgr()
//{
//}
//
//
//GeometryShaderMgr::~GeometryShaderMgr()
//{
//}
