#pragma once
#include "MObjectCtlForm.h"

class MObjectPane : public CDockablePane
{
	DECLARE_DYNAMIC(MObjectPane)
public:
	MObjectCtlForm*  m_wndForm;
public:
	MObjectPane();
	virtual ~MObjectPane();
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
};

