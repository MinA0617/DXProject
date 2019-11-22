#pragma once
#include "MObject.h"



class M2DObject : public MObject
{
protected:
	VertexShader		m_VertexShaderID;
	ID3D11Buffer*		m_pVertexBuffer;	// 오브젝트의 버텍스버퍼
	ID3D11Buffer*		m_pIndexBuffer;
protected:
	D3DXVECTOR2				m_ScreenPosition;
	float				m_ScreenRotation;
	D3DXVECTOR2				m_ScreenScale;
public:
	void				SetPosition(D3DXVECTOR2 pos);
	void				SetRotation(float rot);
	void				SetScale(D3DXVECTOR2 scl);

	void				ModifyPosition(D3DXVECTOR2 pos);
	void				ModifyRotation(float rot);
	void				ModifyScale(D3DXVECTOR2 scl);
public:
	virtual bool		Load(M_STR filename);
	virtual bool		Create(UINT width, UINT height);
public:
	virtual bool		Init();
	virtual bool		Frame();
	virtual bool		Render();
	virtual bool		Release();
	M2DObject();
	~M2DObject();
};

