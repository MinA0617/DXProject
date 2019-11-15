#include "MTexture.h"



MTexture::MTexture()
{
	m_pTexture = NULL;
}


MTexture::~MTexture()
{
	Release();
}

bool MTexture::Load(M_STR filename)
{
	HRESULT TextureLoadResult;
	//// 디바이스객체, 파일이름, 마테리얼속성, 펌프인터페이스, 텍스쳐리소스, 결과값,
	TextureLoadResult = D3DX11CreateShaderResourceViewFromFile(g_pDevice, filename.c_str(), NULL, NULL, &m_pTexture, NULL);
	if (FAILED(TextureLoadResult)) return false;
	//// ID3D11Resource로 받은 텍스쳐를 ID3D11Texture2D로 다운캐스팅 ////
	//pRes->QueryInterface(__uuidof(ID3D11ShaderResourceView), (LPVOID*)&m_pTexture);
	//pRes->Release(); // 안쓰는 부모형은 릴리즈
	return true;
}


bool MTexture::Release()
{
	if (m_pTexture)m_pTexture->Release();
	m_pTexture = NULL;

	return true;
}

ID3D11ShaderResourceView* MTexture::GetTexture()
{
	return m_pTexture;
}

MSize MTexture::GetTextureSize()
{
	ID3D11Texture2D *data;
	m_pTexture->GetResource((ID3D11Resource**)&data);
	D3D11_TEXTURE2D_DESC desc;
	data->GetDesc(&desc);

	return MSize(desc.Width, desc.Height);
}

M_STR MTexture::GetName()
{
	return m_szName;
}
