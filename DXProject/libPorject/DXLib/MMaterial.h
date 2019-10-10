#pragma once
#include "MStd.h"
#include "MTextureMgr.h"
#include "PixelShaderMgr.h"
#include "VertexShaderMgr.h"
#include "MDxState.h"

enum MAPTYPE {NOTFOUND, DIFFUSE, NORMAL, SPECULAR, OPACITY, REFLECTION, REFRACTION, ENVIRONMENT};

class MMaterial
{
public:
	CONSTANT_Matrial	m_ConstantMatrial;
	DWORD				Diffuse;
	DWORD				Normal;
	DWORD				Specular;
	DWORD				Opacity;
	DWORD				Reflection;
	DWORD				Refraction;
	DWORD				Environment;
public:
	BlendState			m_BlendStateID;
	SamplerState		m_SamplerStateID;
	DepthStencilState	m_DepthStencilStateID;
public:
	VertexShader		m_VertexShaderID;
	PixelShader			m_PixelShaderID;
public:
	ID3D11Buffer*		m_pConstantBuffer;
	bool				Load(M_STR NewFileName, MAPTYPE maptype);
	virtual bool		CreateConstantBuffer();
	virtual bool		Init();
	virtual bool		Frame(); // �Ŵ������� �����Ӹ��� 1��ȸ
	virtual bool		Render(); // �� ���׸����� ���� ������Ʈ���� �����Ҷ� ȣ��
	virtual bool		Release();
public:
	MMaterial();
	~MMaterial();
};

