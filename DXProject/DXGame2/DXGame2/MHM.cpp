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
		{"POSITION"	, 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{"WORLDPOS"	, 0, DXGI_FORMAT_R32G32_FLOAT, 1, 0, D3D11_INPUT_PER_INSTANCE_DATA, 0 },
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
// ---------------------------------------------------------
	LoadShaderResult = D3DX11CompileFromFile(L"../../data/Shader/test/DS3D.dsh", NULL, NULL, "DS", "ds_5_0", dwShaderFlags, 0, NULL, &pBlob, &pErrorMsgs, NULL);
	if (FAILED(LoadShaderResult))
	{
		MessageBoxA(g_hWnd, (char*)pErrorMsgs->GetBufferPointer(), "Error", MB_OK);
		return false;
	}
	g_pDevice->CreateDomainShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), NULL, &ds);	// 컴파일된 쉐이더를 생성해 준다
// ---------------------------------------------------------
	LoadShaderResult = D3DX11CompileFromFile(L"../../data/Shader/test/HS3D.hsh", NULL, NULL, "HS", "hs_5_0", dwShaderFlags, 0, NULL, &pBlob, &pErrorMsgs, NULL);
	if (FAILED(LoadShaderResult))
	{
		MessageBoxA(g_hWnd, (char*)pErrorMsgs->GetBufferPointer(), "Error", MB_OK);
		return false;
	}
	g_pDevice->CreateHullShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), NULL, &hs);	// 컴파일된 쉐이더를 생성해 준다

	return true;
}

bool MHM::CreateBuffer()
{
	D3DXVECTOR3 pos[4];
	pos[0] = { -99,0,99 };
	pos[1] = { 99,0,99 };
	pos[2] = { 99,0,-99 };
	pos[3] = { -99,0,-99 };

	DWORD index[4] = { 0,1,2,3 };

	D3DXVECTOR2 worldpos[10];
	for (int i = 0; i < 10; i++)
	{
		worldpos[i] = D3DXVECTOR2(i * 200, i * 200);
	}
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	D3D11_BUFFER_DESC BufferDesc;				//// 공용 버퍼 데스크
	D3D11_SUBRESOURCE_DATA SubresourceData;		//// 공용 리소스 데이터
	HRESULT CreateBufferResult;					//// 공용 결과값
	///// 버텍스 버퍼 ///////////////////////////////////////////////////////////////////////////////////////////////////
	ZeroMemory(&BufferDesc, sizeof(D3D11_BUFFER_DESC));
	BufferDesc.ByteWidth = sizeof(D3DXVECTOR3) * 4;		//// 사이즈 (Byte단위)
	BufferDesc.Usage = D3D11_USAGE_DEFAULT;							//// 사용모드 (DEFAULT는 그래픽카드에 생성)
	BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;				//// 어떤 버퍼로 사용되는지
	ZeroMemory(&SubresourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	SubresourceData.pSysMem = pos;
	if (FAILED(CreateBufferResult = g_pDevice->CreateBuffer(&BufferDesc, &SubresourceData, &vb))) return false;
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



	//// 인덱스 버퍼 /////////////////////////////////////////////////////////////////////////////////////////////////////
	ZeroMemory(&BufferDesc, sizeof(D3D11_BUFFER_DESC));
	BufferDesc.ByteWidth = sizeof(DWORD) * 4;			//// 사이즈 (Byte단위)
	BufferDesc.Usage = D3D11_USAGE_DEFAULT;							//// 사용모드 (DEFAULT는 그래픽카드에 생성)
	BufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;					//// 어떤 버퍼로 사용되는지
	ZeroMemory(&SubresourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	SubresourceData.pSysMem = index;
	if (FAILED(CreateBufferResult = g_pDevice->CreateBuffer(&BufferDesc, &SubresourceData, &ib))) return false;
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	///// 인스턴스 버퍼 ///////////////////////////////////////////////////////////////////////////////////////////////////
	ZeroMemory(&BufferDesc, sizeof(D3D11_BUFFER_DESC));
	BufferDesc.ByteWidth = sizeof(D3DXVECTOR2) * 10;		//// 사이즈 (Byte단위)
	BufferDesc.Usage = D3D11_USAGE_DEFAULT;							//// 사용모드 (DEFAULT는 그래픽카드에 생성)
	BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;				//// 어떤 버퍼로 사용되는지
	ZeroMemory(&SubresourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	SubresourceData.pSysMem = worldpos;
	if (FAILED(CreateBufferResult = g_pDevice->CreateBuffer(&BufferDesc, &SubresourceData, &ivb))) return false;
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	return true;
}

bool MHM::Render()
{
	if (I_DxState.m_bIsSolid)
	{
		if (m_bIsCulling)
		{
			I_DxState.RS_Set(MSolidFrame);
		}
		else
		{
			I_DxState.RS_Set(MNoneCulling);
		}
	}
	else
	{
		I_DxState.RS_Set(MWireFrame);
	}

	I_DxState.BS_Set(MAlphaBlend);
	I_DxState.SS_Set(MWrapLinear);
	I_DxState.DSS_Set(MDepthEnable);

	//ID3D11ShaderResourceView* temp = I_TextureMgr[0]->GetTexture();
	//g_pImmediateContext->PSSetShaderResources(0, 1, &temp);

	g_pImmediateContext->PSSetShader(ps, NULL, 0);

	//g_pImmediateContext->PSSetConstantBuffers(1, 1, &m_pConstantBuffer);
	g_pImmediateContext->IASetInputLayout(lo);		// 레이아웃 셋
	g_pImmediateContext->VSSetShader(vs, NULL, 0);
	g_pImmediateContext->HSSetShader(hs, NULL, 0);
	g_pImmediateContext->DSSetShader(ds, NULL, 0);
	g_pImmediateContext->PSSetShader(ps, NULL, 0);
	UINT stride = sizeof(D3DXVECTOR3);		//// 보폭(카운트할 사이즈 크기)
	UINT offset = 0;
	g_pImmediateContext->IASetVertexBuffers(0, 1, &vb, &stride, &offset);
	stride = sizeof(D3DXVECTOR2);
	g_pImmediateContext->IASetVertexBuffers(1, 1, &ivb, &stride, &offset);
	//g_pImmediateContext->IASetIndexBuffer(ib, DXGI_FORMAT_R32_UINT, 0);
	g_pImmediateContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_4_CONTROL_POINT_PATCHLIST);
	//g_pImmediateContext->Draw(4, 0);
	g_pImmediateContext->DrawInstanced(4, 10, 0, 0);
	//g_pImmediateContext->DrawIndexedInstanced(1, 10, 0, 0, 0);
	g_pImmediateContext->HSSetShader(NULL, NULL, 0);
	g_pImmediateContext->DSSetShader(NULL, NULL, 0);
	return true;
}

bool MHM::Release()
{
	SAFE_RELEASE(vb);
	SAFE_RELEASE(ivb);
	SAFE_RELEASE(ib);
	SAFE_RELEASE(lo);
	SAFE_RELEASE(vs);
	SAFE_RELEASE(hs);
	SAFE_RELEASE(ds);
	SAFE_RELEASE(ps);
	return true;
}

MHM::MHM()
{
}


MHM::~MHM()
{
}
