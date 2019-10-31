#pragma once
#include "MObject.h"
#include "MMeshMgr.h"
#include "MKeyData.h"
#include "MBoundingBox.h"

class M3DObject : public MObject
{
	friend class MBoundingBox;
public:
	//DWORD				ObjID; // 오브젝트매니져용 ID값 없을경우 0
	MMesh*				m_pObj;
public:
	CONSTANT_3DOBJ		m_ConstantOBJ;
	vector<MBoundingBox*>	m_BoxList;
	//MKeyData*			m_KeyData;
	//virtual bool		UpdateKey(float time);
	//--------------------------------------------------
protected:
	D3DXVECTOR3			m_WorldPosition;
	D3DXQUATERNION		m_WorldRotation;
	D3DXVECTOR3			m_WorldScale;
	//--------------------------------------------------
	D3DXVECTOR3			m_LocalPosition;
	D3DXQUATERNION		m_LocalRotation;
	D3DXVECTOR3			m_LocalScale;
	//--------------------------------------------------
protected:
	M3DObject*			m_pParents;
	list<M3DObject*>	m_pChildList;
public:
	M3DObject*			GetParents();
	bool				m_bIsInherityPosition;
	bool				m_bIsInherityRotation;
	bool				m_bIsInherityScale;
public:
	void				Copy(M3DObject* target);
	D3DXVECTOR3			GetLocalPosition();
	D3DXQUATERNION		GetLocalRotation();
	D3DXVECTOR3			GetLocalScale();
	//--------------------------------------------------
	virtual bool		CreateConstantBuffer();
	//--------------------------------------------------
	D3DXMATRIX			GetPositionMatrix();
	D3DXMATRIX			GetRotationMatrix();
	D3DXMATRIX			GetScaleMatrix();
	CONSTANT_3DOBJ		GetConstantOBJ();
public:
	//--------------------------------------------------
	bool				IsHasParents();
	void				LinkParents(M3DObject* Parents);
	void				UnLinkParents();
	//--------------------------------------------------
	virtual void		SetLocalPosition(D3DXVECTOR3 data);
	virtual void		SetLocalRotation(D3DXQUATERNION data);
	virtual void		SetLocalScale(D3DXVECTOR3 data);
	//--------------------------------------------------
	virtual void		ModifyLocalPosition(D3DXVECTOR3 data);
	virtual void		ModifyLocalRotation(D3DXQUATERNION data);
	virtual void		ModifyLocalScale(D3DXVECTOR3 data);
	//--------------------------------------------------
	virtual void		SetColor(D3DXVECTOR3 data);
	virtual void		UpdateBox();
public:
	virtual bool		Init();
	virtual bool		Frame();
	virtual bool		Render();
	virtual bool		Release();
public:
	virtual bool		PreFrame() final; //// 상수버퍼 갱신
public:
	M3DObject();
	~M3DObject();
};

