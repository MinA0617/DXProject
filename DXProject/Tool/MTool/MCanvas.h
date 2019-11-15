#pragma once
#include "MSelect.h"
class MCanvas
{
private:
	//ID3D11Texture2D*	m_pAlphaTexture;
	//UINT				m_iSize;			//가로세로가 동일함. 픽셀사이즈
	UINT				m_iRadius;			//지름
public:
	//bool				Create();
	bool				Brushing(UINT layer);
public:
	MCanvas();
	~MCanvas();
};

