#pragma once
#include "MMesh.h"
#include "MMaterial.h"
#include "M3DInstance.h"

class M3DInstanceModel
{
public:
	// -------------------------------------
	//	���� ������
	M_STR					m_name;
	MMesh*					m_Mesh;
	MMaterial*				m_Material;
	MBoundingBox			m_Box;			//�ν��Ͻ� ������ ����� ������
	// -------------------------------------
	//	�ν��Ͻ� ������
	map<int, M3DNObject*>	m_InstanceList;
	vector<D3DXMATRIX>		m_MatrixList;
	ID3D11Buffer*			m_InstanceBuffer;
	// -------------------------------------
	int						m_iCount;
	int						m_iSize;
	// -------------------------------------
public:
	virtual bool			CreateInstanceBuffer();
public:
	bool					Load_MAP(M_STR filename, MAPTYPE type = NOTFOUND);
	int						CreateInstanceObject(int count = 1);
	M3DNObject*				GetInstanceObject(int index);
	bool					DeleteInstanceObject(int index);
	virtual	bool			AddRenderObject(M3DInstance* obj);
public:
	virtual bool			Init();
	virtual bool			Frame();
	virtual bool			Render();
	virtual bool			Release();
public:
	M3DInstanceModel();
	virtual ~M3DInstanceModel();
};

