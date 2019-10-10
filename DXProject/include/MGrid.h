#pragma once
#include "M3DSpline.h"
class MGrid : public M3DSpline
{
public:
	UINT			m_GridSize; ///// ���μ��� ������
	UINT			m_StepSize; ///// ���ǻ�����
public:
	bool			SetGrid(UINT GridSize, UINT StepSize);
public:
	virtual bool	Init();
	//virtual bool	Frame();
public:
	MGrid();
	~MGrid();
};

