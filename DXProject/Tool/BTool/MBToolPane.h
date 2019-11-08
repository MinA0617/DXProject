#pragma once
#include "MBForm.h"

class MBToolPane : public CDockablePane
{
	DECLARE_DYNAMIC(MBToolPane)
public:
	MBForm*  m_wndForm;
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
public:
	MBToolPane();
	virtual ~MBToolPane();
};

