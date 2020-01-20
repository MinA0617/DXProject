#pragma once
#include "MStd.h"
#include "D3Dcompiler.h"
#define PS_NUM 7

enum PixelShader { PS2D, PS3D, PSSPLINE, PSBONE, PSFILED, PS3DINSTANCE, PS3DSKININSTANCE };

class PixelShaderMgr : public MSingleton<PixelShaderMgr>
{
	friend class MSingleton<PixelShaderMgr>;
public:
	ID3D11PixelShader*		m_PSList[PS_NUM];
public:
	bool					Init();
	bool					Release();
private:
	PixelShaderMgr();
public:
	~PixelShaderMgr();
};

#define I_PixelShaderMgr PixelShaderMgr::GetInstance()