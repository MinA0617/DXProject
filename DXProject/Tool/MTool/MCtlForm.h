#pragma once

class MCtlForm : public CFormView
{
	DECLARE_DYNAMIC(MCtlForm)
public:
	MCtlForm();
	virtual ~MCtlForm();
	static MCtlForm* CreateOne(CWnd* pParent);
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CtlForm };
#endif
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedLod();
	afx_msg void OnBnClickedSolid();
	bool m_bIsLod;
	bool m_bIsSoild;
};

