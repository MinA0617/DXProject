#pragma once
#include "MStd.h"
#include "D3Dcompiler.h"
#define VS_NUM 5

enum VertexShader { VS2D, VS3D, VSSPLINE, VSCHARACTER, VSFILED };

class VertexShaderMgr : public MSingleton<VertexShaderMgr>
{
	friend class MSingleton<VertexShaderMgr>;
public:
	ID3D11VertexShader*		m_VSList[VS_NUM];
	ID3D11InputLayout*		m_LOList[VS_NUM];
	public:
	bool					Init();
	bool					Release();
private:
	VertexShaderMgr();
public:
	~VertexShaderMgr();
};

#define I_VertexShaderMgr VertexShaderMgr::GetInstance()