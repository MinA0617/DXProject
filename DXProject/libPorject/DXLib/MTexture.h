#pragma once
#include "MStd.h"

class MTexture
{
	friend class MTextureMgr;
private:
	ID3D11ShaderResourceView*	m_pTexture; // ¸®¼Ò½º
	M_STR						m_szName;
	M_STR						m_szPath;
public:
	ID3D11ShaderResourceView*	GetTexture();
	MSize						GetTextureSize();
	M_STR						GetName();
	M_STR						GetPath();
	bool						Load(M_STR filename);
	bool						Release();
public:
	MTexture();
	~MTexture();
};

