#include "stdafx.h"
#include "MCanvas.h"

bool MCanvas::Init()
{
#pragma region VSPS
	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined(DEBUG) || defined(_DEBUG)
	dwShaderFlags |= D3DCOMPILE_DEBUG;
#endif

	HRESULT LoadShaderResult;
	ID3D10Blob* pVSShader;
	ID3D10Blob* pPSShader;
	ID3D10Blob* pErrorMsgs;
	LoadShaderResult = D3DX11CompileFromFile(L"../brush/canvas.vsh", NULL, NULL, "VS", "vs_5_0", dwShaderFlags, 0, NULL, &pVSShader, &pErrorMsgs, NULL);
	if (FAILED(LoadShaderResult))
	{
		MessageBoxA(g_hWnd, (char*)pErrorMsgs->GetBufferPointer(), "Error", MB_OK);
		return false;
	}
	g_pDevice->CreateVertexShader(pVSShader->GetBufferPointer(), pVSShader->GetBufferSize(), NULL, &m_VS);	// 컴파일된 쉐이더를 생성해 준다
	const D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{"DATA"	, 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	int layoutNum = _countof(layout);
	LoadShaderResult = g_pDevice->CreateInputLayout(layout, layoutNum, pVSShader->GetBufferPointer(), pVSShader->GetBufferSize(), &m_LO);
	if (pVSShader)pVSShader->Release();
	if (pErrorMsgs)pErrorMsgs->Release();
	if (FAILED(LoadShaderResult)) return false;

	LoadShaderResult = D3DX11CompileFromFile(L"../brush/canvas.vsh", NULL, NULL, "PS", "ps_5_0", dwShaderFlags, 0, NULL, &pPSShader, &pErrorMsgs, NULL);
	if (FAILED(LoadShaderResult))
	{

		MessageBoxA(g_hWnd, (char*)pErrorMsgs->GetBufferPointer(), "Error", MB_OK);
		return false;
	}
	g_pDevice->CreatePixelShader(pPSShader->GetBufferPointer(), pPSShader->GetBufferSize(), NULL, &m_PS);	// 컴파일된 쉐이더를 생성해 준다

	LoadShaderResult = D3DX11CompileFromFile(L"../brush/eraser.vsh", NULL, NULL, "PS", "ps_5_0", dwShaderFlags, 0, NULL, &pPSShader, &pErrorMsgs, NULL);
	if (FAILED(LoadShaderResult))
	{

		MessageBoxA(g_hWnd, (char*)pErrorMsgs->GetBufferPointer(), "Error", MB_OK);
		return false;
	}
	g_pDevice->CreatePixelShader(pPSShader->GetBufferPointer(), pPSShader->GetBufferSize(), NULL, &m_ERASER);	// 컴파일된 쉐이더를 생성해 준다
#pragma endregion VSPS
#pragma region BUFFER
	D3DXVECTOR4 vertices[6];
	vertices[0] = D3DXVECTOR4(-1, 1, 0, 0);
	vertices[1] = D3DXVECTOR4(1, 1, 1, 0);
	vertices[2] = D3DXVECTOR4(-1, -1, 0, 1);
	vertices[3] = D3DXVECTOR4(1, 1, 1, 0);
	vertices[4] = D3DXVECTOR4(1, -1, 1, 1);
	vertices[5] = D3DXVECTOR4(-1, -1, 0, 1);
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	D3D11_BUFFER_DESC BufferDesc;				//// 공용 버퍼 데스크
	D3D11_SUBRESOURCE_DATA SubresourceData;		//// 공용 리소스 데이터
	HRESULT CreateBufferResult;					//// 공용 결과값
	///// 버텍스 버퍼 ///////////////////////////////////////////////////////////////////////////////////////////////////
	ZeroMemory(&BufferDesc, sizeof(D3D11_BUFFER_DESC));
	BufferDesc.ByteWidth = sizeof(D3DXVECTOR4) * 6;		//// 사이즈 (Byte단위)
	BufferDesc.Usage = D3D11_USAGE_DEFAULT;							//// 사용모드 (DEFAULT는 그래픽카드에 생성)
	BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;				//// 어떤 버퍼로 사용되는지
	ZeroMemory(&SubresourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	SubresourceData.pSysMem = vertices;
	if (FAILED(CreateBufferResult = g_pDevice->CreateBuffer(&BufferDesc, &SubresourceData, &m_pBuffer))) return false;
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	ZeroMemory(&m_ConstantOBJ, sizeof(m_ConstantOBJ));
	m_ConstantOBJ.ScreenScale = D3DXVECTOR2(1, 1);

	ZeroMemory(&BufferDesc, sizeof(D3D11_BUFFER_DESC));
	BufferDesc.ByteWidth = sizeof(CONSTANT_2DOBJ);
	BufferDesc.Usage = D3D11_USAGE_DEFAULT;
	BufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

	ZeroMemory(&SubresourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	SubresourceData.pSysMem = &m_ConstantOBJ;

	if (FAILED(CreateBufferResult = g_pDevice->CreateBuffer(&BufferDesc, &SubresourceData, &m_pConstantBuffer))) return false;

#pragma endregion BUFFER
	// -------------------------------------------------------------------
	//	브러쉬 로딩
	// -------------------------------------------------------------------
#pragma region BLEND
	HRESULT hr;
	D3D11_BLEND_DESC ds;
	ZeroMemory(&ds, sizeof(D3D11_BLEND_DESC));
	ds.AlphaToCoverageEnable = TRUE;
	ds.IndependentBlendEnable = TRUE;
	ds.RenderTarget[0].BlendEnable = TRUE;

	ds.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	ds.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	ds.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;

	ds.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	ds.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	ds.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;

	ds.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	if (FAILED(hr = g_pDevice->CreateBlendState(&ds, &m_BS)))
	{
		return false;
	}
	ds.RenderTarget[0].BlendOp = D3D11_BLEND_OP_REV_SUBTRACT;
	ds.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	ds.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;

	ds.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	ds.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	ds.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;

	if (FAILED(hr = g_pDevice->CreateBlendState(&ds, &m_BSERASER)))
	{
		return false;
	}
#pragma endregion BLEND

	//DWORD index = I_TextureMgr.Load(L"../brush/defult.bmp");
	//if (index) m_BrushList.push_back(index);
	//index = I_TextureMgr.Load(L"../brush/defult2.bmp");
	//if (index) m_BrushList.push_back(index);
	return true;
}

bool MCanvas::Frame()
{
	if (m_pConstantBuffer)
	{
		g_pImmediateContext->UpdateSubresource(m_pConstantBuffer, 0, 0, &m_ConstantOBJ, 0, 0);
	}
	return true;
}

bool MCanvas::LoadBrush(M_STR filepath)
{
	int	index = I_TextureMgr.Load(filepath);
	if (index)
	{
		m_BrushList.push_back(index);
		return true;
	}
	return false;
}

bool MCanvas::Create(float xsize, float ysize)
{
	if (!I_3DObjectMgr.m_InWorldFiled) return false;
	M3DHeightMap* hm = I_3DObjectMgr.m_InWorldFiled->ground;
	if (!hm->m_pAlphaTexture) return false;

	HRESULT hr;
	D3D11_TEXTURE2D_DESC td;
	ZeroMemory(&td, sizeof(D3D11_TEXTURE2D_DESC));
	td.Width = xsize;
	td.Height = ysize;
	td.MipLevels = 1;
	td.SampleDesc.Count = 1;
	td.SampleDesc.Quality = 0;
	td.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	td.Usage = D3D11_USAGE_DEFAULT;
	td.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	td.ArraySize = 1;
	hr = g_pDevice->CreateTexture2D(&td, NULL, &m_pCanvasTexture);
	if (FAILED(hr)) return false;
	hr = g_pDevice->CreateShaderResourceView(m_pCanvasTexture, NULL, &m_pShaderResourceView);
	if (FAILED(hr)) return false;
	hr = g_pDevice->CreateRenderTargetView(m_pCanvasTexture, NULL, &m_pRenderTargetView);
	if (FAILED(hr)) return false;

	m_ViewPort.Width = xsize;
	m_ViewPort.Height = ysize;
	m_ViewPort.MinDepth = 0;
	m_ViewPort.MaxDepth = 1;
	m_ViewPort.TopLeftX = 0;
	m_ViewPort.TopLeftY = 0;

	hm->m_pAlphaTexture->GetDesc(&td);
	m_TextureSize.x = td.Width;
	m_TextureSize.y = td.Height;

	SAFE_RELEASE(hm->m_pAlphaTextureSRV);
	hm->m_pAlphaTextureSRV = m_pShaderResourceView;

	return true;
}

bool MCanvas::Brushing()
{
	if (m_BrushID >= m_BrushList.size()) return false;
	if (!I_3DObjectMgr.m_InWorldFiled) return false;
	M3DHeightMap* hm = I_3DObjectMgr.m_InWorldFiled->ground;
	if(!hm->m_pAlphaTexture) return false;
	MRAY ray = MSelect::GetScreenRay();
	MTreeNode* pNode = MSelect::CheckNode(I_3DObjectMgr.m_pTree->m_pRootNode, &ray);
	D3DXVECTOR3 center;
	DWORD		index;
	int	XCount = I_3DObjectMgr.m_InWorldFiled->ground->m_iCount;
	if (pNode)
	{
		if (MSelect::CheckTri(pNode, &ray, &center, &index))
		{
			float x = (center.x / (hm->m_fLeafSize * hm->m_iCount)) * 2;
			float y = (center.z / (hm->m_fLeafSize * hm->m_iCount)) * 2;
			m_ConstantOBJ.ScreenScale = D3DXVECTOR2(m_iRadius, m_iRadius);
			m_ConstantOBJ.ScreenPosition = D3DXVECTOR2((x * m_TextureSize.x) / 2, -(y * m_TextureSize.y) / 2);
			m_ConstantOBJ.ScreenRotation = m_fOpacity;
			m_ConstantOBJ.empty1 = m_TextureSize.x;
			m_ConstantOBJ.empty2 = m_TextureSize.y;
			m_ConstantOBJ.empty3 = m_Channel;

			Begin();
			Frame();
			Render();
			End();

			//SAFE_RELEASE(hm->m_pAlphaTextureSRV);
			//HRESULT hr = g_pDevice->CreateShaderResourceView(m_pCanvasTexture, NULL, &hm->m_pAlphaTextureSRV);
			//if (FAILED(hr)) return false;

		}
		return true;
	}
}

bool MCanvas::Begin()
{
	I_DxState.RS_Set(MNoneCulling);

	m_iViewports = 1;
	g_pImmediateContext->RSGetViewports(&m_iViewports, &m_OldViewPort);
	g_pImmediateContext->OMGetRenderTargets(1, &m_pOldRenderTargetView, &m_pOldDepthStencilView);

	ID3D11RenderTargetView* temp = NULL;
	ID3D11DepthStencilView* temp2 = NULL;

	g_pImmediateContext->OMSetRenderTargets(1, &temp, temp2);
	g_pImmediateContext->OMSetRenderTargets(1, &m_pRenderTargetView, NULL);
	g_pImmediateContext->RSSetViewports(1, &m_ViewPort);
	return true;
}

bool MCanvas::End()
{
	ID3D11RenderTargetView* temp = NULL;
	ID3D11DepthStencilView* temp2 = NULL;
	g_pImmediateContext->OMSetRenderTargets(1, &temp, temp2);

	g_pImmediateContext->RSSetViewports(m_iViewports, &m_OldViewPort);
	g_pImmediateContext->OMSetRenderTargets(1, &m_pOldRenderTargetView, m_pOldDepthStencilView);


	m_pOldRenderTargetView = NULL;
	m_pOldDepthStencilView = NULL;


	I_DxState.RS_Set(MSolidFrame);
	return true;
}

bool MCanvas::Render()
{
	I_DxState.RS_Set(MNoneCulling);

	I_DxState.SS_Set(MWrapLinear);
	I_DxState.DSS_Set(MDepthDisable);

	ID3D11ShaderResourceView* temp = I_TextureMgr[m_BrushList[m_BrushID]]->GetTexture();
	if (m_bIsEraser)
	{
		g_pImmediateContext->PSSetShader(m_ERASER, NULL, 0);
		g_pImmediateContext->OMSetBlendState(m_BSERASER, 0, 0xffffffff);
	}
	else
	{
		g_pImmediateContext->PSSetShader(m_PS, NULL, 0);
		g_pImmediateContext->OMSetBlendState(m_BS, 0, 0xffffffff);
	}
	g_pImmediateContext->PSSetShaderResources(0, 1, &temp);
	g_pImmediateContext->PSSetConstantBuffers(0, 1, &m_pConstantBuffer);

	g_pImmediateContext->IASetInputLayout(m_LO);		// 레이아웃 셋

	g_pImmediateContext->VSSetShader(m_VS, NULL, 0);
	g_pImmediateContext->VSSetConstantBuffers(0, 1, &m_pConstantBuffer);

	UINT stride = sizeof(D3DXVECTOR4);		//// 보폭(카운트할 사이즈 크기)
	UINT offset = 0;
	g_pImmediateContext->IASetVertexBuffers(0, 1, &m_pBuffer, &stride, &offset);
	g_pImmediateContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	g_pImmediateContext->Draw(6, 0);
	return true;
}

bool MCanvas::Release()
{
	SAFE_RELEASE(m_pCanvasTexture);
	SAFE_RELEASE(m_pRenderTargetView);
	return true;
}

MCanvas::MCanvas()
{
	m_iRadius = 100;
	m_Channel = 0;
	m_bIsEraser = false;
	m_fOpacity = 1;
}


MCanvas::~MCanvas()
{
	Release();
}
