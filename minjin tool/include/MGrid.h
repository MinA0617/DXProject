#pragma once
#include "M3DSpline.h"
class MGrid : public M3DSpline
{
public:
	UINT			m_GridSize; ///// 가로세로 사이즈
	UINT			m_StepSize; ///// 스탭사이즈
public:
	bool			SetGrid(UINT GridSize, UINT StepSize);
public:
	virtual bool	Init();
	//virtual bool	Frame();
public:
	MGrid();
	~MGrid();
};

