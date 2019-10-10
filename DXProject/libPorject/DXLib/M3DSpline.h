#pragma once
#include "M3DObject.h"

class M3DSpline : public M3DObject
{
protected:
	ID3D11Buffer*		m_pVertexBuffer;
	ID3D11Buffer*		m_pIndexBuffer;
	UINT				m_iVertexCount;
	UINT				m_iIndexCount;
public:
	virtual bool	Load(SPLINE_VERTEX* data, UINT iCount);
	virtual bool	Init();
	virtual bool	Frame();
	virtual bool	Render();
	virtual bool	Release();
public:
public:
	M3DSpline();
	~M3DSpline();
};

