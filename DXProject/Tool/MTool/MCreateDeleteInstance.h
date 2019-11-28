#pragma once
#include "MSelect.h"
class MCreateDeleteInstance
{
public:
	int		m_iID;
	float	m_fRotationX;
	float	m_fRotationY;
	float	m_fRotationZ;
	float	m_fScale;
	int		m_iRotationVariationX;
	int		m_iRotationVariationY;
	int		m_iRotationVariationZ;
	int		m_iScaleVariation;
public:
	bool	Create();
	bool	Delete();
	map<int, M_STR> table;
	map<M_STR, vector<int>> list;
	vector<M_STR> filepath;
	MCreateDeleteInstance();
	~MCreateDeleteInstance();
};

