#pragma once
#include "MSelect.h"
class MCanvas
{
private:
	//ID3D11Texture2D*	m_pAlphaTexture;
	//UINT				m_iSize;			//���μ��ΰ� ������. �ȼ�������
	UINT				m_iRadius;			//����
public:
	//bool				Create();
	bool				Brushing(UINT layer);
public:
	MCanvas();
	~MCanvas();
};

