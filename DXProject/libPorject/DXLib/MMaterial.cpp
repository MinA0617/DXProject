#include "MMaterial.h"
#include "MCameraMgr.h"


void MMaterial::SetOpacity(DWORD level)
{
	m_ConstantMatrial.isOpacity = level;
}

bool MMaterial::Load(M_STR NewFileName, MAPTYPE maptype)
{
	switch (maptype)
	{
	case NOTFOUND:
	{
		return false;
	}
	case DIFFUSE:
	{
		Diffuse = I_TextureMgr.Load(NewFileName);
		if (Diffuse == NULL) return false;
		m_ConstantMatrial.isDiffuse = -1;
		return true;
	}
	case NORMAL:
	{
		Normal = I_TextureMgr.Load(NewFileName);
		if (Normal == NULL) return false;
		m_ConstantMatrial.isNormal = -1;
		return true;
	}
	case SPECULAR:
	{
		Specular = I_TextureMgr.Load(NewFileName);
		if (Specular == NULL) return false;
		m_ConstantMatrial.isSpecular = -1;
		return true;
	}
	case OPACITY:
	{
		Opacity = I_TextureMgr.Load(NewFileName);
		if (Opacity == NULL) return false;
		m_ConstantMatrial.isOpacity = -1;
		return true;
	}
	case REFLECTION:
	{
		Reflection = I_TextureMgr.Load(NewFileName);
		if (Reflection == NULL) return false;
		m_ConstantMatrial.isReflection = -1;
		return true;
	}
	case REFRACTION:
	{
		Refraction = I_TextureMgr.Load(NewFileName);
		if (Refraction == NULL) return false;
		m_ConstantMatrial.isRefraction = -1;
		return true;
	}
	case ENVIRONMENT:
	{
		Environment = I_TextureMgr.Load(NewFileName);
		if (Environment == NULL) return false;
		m_ConstantMatrial.isEnvironment = -1;
		return true;
	}
	default:
		return false;
	}
}

bool MMaterial::CreateConstantBuffer()
{
	D3D11_BUFFER_DESC BufferDesc;
	HRESULT CreateBufferResult;
	D3D11_SUBRESOURCE_DATA SubresourceData;

	ZeroMemory(&BufferDesc, sizeof(D3D11_BUFFER_DESC));
	BufferDesc.ByteWidth = sizeof(CONSTANT_Matrial);
	BufferDesc.Usage = D3D11_USAGE_DEFAULT;
	BufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

	ZeroMemory(&SubresourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	SubresourceData.pSysMem = &m_ConstantMatrial;

	if (FAILED(CreateBufferResult = g_pDevice->CreateBuffer(&BufferDesc, &SubresourceData, &m_pConstantBuffer))) return false;
	return true;
}

bool MMaterial::Init()
{
	// 상수버퍼 생성
	m_BlendStateID = MAlphaBlend;
	m_SamplerStateID = MWrapLinear;
	m_DepthStencilStateID = MDepthEnable;

	m_PixelShaderID = PS3D;

	ZeroMemory(&m_ConstantMatrial, sizeof(m_ConstantMatrial));
	m_ConstantMatrial.ObjectColor = D3DXVECTOR3(1,1,1);
	m_ConstantMatrial.isOpacity = 255;

	CreateConstantBuffer();

	return true;
}

bool MMaterial::Frame()
{
	// 상수버퍼 갱신
	if(m_pConstantBuffer) g_pImmediateContext->UpdateSubresource(m_pConstantBuffer, 0, 0, &m_ConstantMatrial, 0, 0);
	return true;
}

bool MMaterial::Render()
{
	// 상수버퍼, 쉐이더ID 바인딩
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

	I_DxState.BS_Set(m_BlendStateID);
	I_DxState.SS_Set(m_SamplerStateID);
	I_DxState.DSS_Set(m_DepthStencilStateID);

	#pragma region MapBinding
	if (m_ConstantMatrial.isDiffuse == -1)
	{
		ID3D11ShaderResourceView* temp = I_TextureMgr[Diffuse]->GetTexture();
		g_pImmediateContext->PSSetShaderResources(0, 1, &temp);
	}
	if (m_ConstantMatrial.isNormal == -1)
	{
		ID3D11ShaderResourceView* temp = I_TextureMgr[Normal]->GetTexture();
		g_pImmediateContext->PSSetShaderResources(1, 1, &temp);
	}
	if (m_ConstantMatrial.isEnvironment == -1)
	{
		ID3D11ShaderResourceView* temp = I_TextureMgr[Environment]->GetTexture();
		g_pImmediateContext->PSSetShaderResources(2, 1, &temp);
	}
	if (m_ConstantMatrial.isSpecular == -1)
	{
		ID3D11ShaderResourceView* temp = I_TextureMgr[Specular]->GetTexture();
		g_pImmediateContext->PSSetShaderResources(3, 1, &temp);
	}
	if (m_ConstantMatrial.isOpacity == -1)
	{
		ID3D11ShaderResourceView* temp = I_TextureMgr[Opacity]->GetTexture();
		g_pImmediateContext->PSSetShaderResources(4, 1, &temp);
	}
	if (m_ConstantMatrial.isReflection == -1)
	{
		ID3D11ShaderResourceView* temp = I_TextureMgr[Reflection]->GetTexture();
		g_pImmediateContext->PSSetShaderResources(5, 1, &temp);
	}
	if (m_ConstantMatrial.isRefraction == -1)
	{
		ID3D11ShaderResourceView* temp = I_TextureMgr[Refraction]->GetTexture();
		g_pImmediateContext->PSSetShaderResources(6, 1, &temp);
	}
	#pragma endregion

	g_pImmediateContext->PSSetShader(I_PixelShaderMgr.m_PSList[m_PixelShaderID], NULL, 0);

	g_pImmediateContext->PSSetConstantBuffers(0, 1, &I_CameraMgr.m_pGrobalCameraBuffer);
	g_pImmediateContext->PSSetConstantBuffers(1, 1, &m_pConstantBuffer);
	return true;
}

bool MMaterial::Release()
{
	// 상수버퍼 해제
	SAFE_RELEASE(m_pConstantBuffer);
	return true;
}

MMaterial::MMaterial()
{
	m_bIsCulling = true;
}


MMaterial::~MMaterial()
{
}
