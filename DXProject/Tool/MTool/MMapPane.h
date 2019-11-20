#pragma once
#include "MMapCtlForm.h"

class MMapPane : public CDockablePane
{
	DECLARE_DYNAMIC(MMapPane)
public:
	MMapCtlForm*  m_wndForm;
public:
	MMapPane();
	virtual ~MMapPane();
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
};

