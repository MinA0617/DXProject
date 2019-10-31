#pragma once
#include "MObject.h"



class M2DObject : public MObject
{
protected:
	VertexShader		m_VertexShaderID;
	ID3D11Buffer*		m_pVertexBuffer;	// 오브젝트의 버텍스버퍼
	ID3D11Buffer*		m_pIndexBuffer;
protected:
	MPoint				m_ScreenPosition;
	float				m_ScreenRotation;
	MPoint				m_ScreenScale;
public:
	void				SetPosition(MPoint pos);
	void				SetRotation(float rot);
	void				SetScale(MPoint scl);

	void				ModifyPosition(MPoint pos);
	void				ModifyRotation(float rot);
	void				ModifyScale(MPoint scl);
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

