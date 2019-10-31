#pragma once
#include "MStd.h"
#include "MTextureMgr.h"
#include "PixelShaderMgr.h"
#include "VertexShaderMgr.h"
#include "MDxState.h"

enum MAPTYPE {NOTFOUND, DIFFUSE, NORMAL, SPECULAR, OPACITY, REFLECTION, REFRACTION, ENVIRONMENT};

class MMaterial
{
	friend class M2DObject;
	friend class M3DObject;
private:
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
	PixelShader			m_PixelShaderID;
public:
	void				SetOpacity(DWORD level);
	//bool				SetOpacity(M_STR filename);
public:
	ID3D11Buffer*		m_pConstantBuffer;
	bool				Load(M_STR NewFileName, MAPTYPE maptype);
	virtual bool		CreateConstantBuffer();
	virtual bool		Init();
	virtual bool		Frame(); // 매니져에서 프레임마다 1순회
	virtual bool		Render(); // 이 마테리얼을 가진 오브젝트에서 랜더할때 호출
	virtual bool		Release();
public:
	MMaterial();
	~MMaterial();
};

