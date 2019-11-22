#pragma once
#include "MStd.h"
#include "D3Dcompiler.h"


class MHM
{
public:
	ID3D11InputLayout*		lo;
	ID3D11VertexShader*		vs;
	ID3D11HullShader*		hs;
	ID3D11DomainShader*		ds;
	ID3D11PixelShader*		ps;
	bool				Init();
	MHM();
	~MHM();
};

