#pragma once
#include "MObject.h"



class M2DObj : public MObject
{
private:
	DWORD				MapID;
private:
	DWORD				VertexShaderID;
	DWORD				PixelShaderID;
	DWORD				BlendStateID;
private:
	ID3D11Buffer*		m_pVertexBuffer;	// 오브젝트의 버텍스버퍼
	ID3D11Buffer*		m_pIndexBuffer;
	ID3D11Buffer*		m_pConstantBuffer;
private:
	MPoint				m_ScreenPosition;
	float				m_ScreenRotation;
	MPoint				m_ScreenScale;
public:
	bool				SetShader(int VS, int PS);

	void				SetPosition(MPoint pos);
	void				SetRotation(float rot);
	void				SetScale(MPoint scl);

	void				ModifyPosition(MPoint pos);
	void				ModifyRotation(float rot);
	void				ModifyScale(MPoint scl);

public:
	virtual bool		Load(M_STR filename);
public:
	virtual bool		Init();
	virtual bool		Frame();
	virtual bool		Render();
	virtual bool		Release();
	M2DObj();
	~M2DObj();
};

