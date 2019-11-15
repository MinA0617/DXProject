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
	//// ����̽���ü, �����̸�, ���׸���Ӽ�, �����������̽�, �ؽ��ĸ��ҽ�, �����,
	TextureLoadResult = D3DX11CreateShaderResourceViewFromFile(g_pDevice, filename.c_str(), NULL, NULL, &m_pTexture, NULL);
	if (FAILED(TextureLoadResult)) return false;
	//// ID3D11Resource�� ���� �ؽ��ĸ� ID3D11Texture2D�� �ٿ�ĳ���� ////
	//pRes->QueryInterface(__uuidof(ID3D11ShaderResourceView), (LPVOID*)&m_pTexture);
	//pRes->Release(); // �Ⱦ��� �θ����� ������
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
