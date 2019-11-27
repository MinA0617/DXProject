#pragma once
#include "MStd.h"
#include "M3DModel.h"
#include "D3Dcompiler.h"


class MHM
{
public:
	bool					m_bIsCulling;
	ID3D11Buffer*			vb;
	ID3D11Buffer*			ivb;
	ID3D11Buffer*			ib;
	ID3D11InputLayout*		lo;
	ID3D11VertexShader*		vs;
	ID3D11HullShader*		hs;
	ID3D11DomainShader*		ds;
	ID3D11PixelShader*		ps;
	bool				Init();
	bool				CreateBuffer();
	bool				Render();
	bool				Release();
	MHM();
	~MHM();
};

