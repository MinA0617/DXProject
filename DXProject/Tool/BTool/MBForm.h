#pragma once

class MBForm : public CFormView
{
	DECLARE_DYNAMIC(MBForm)
public:
	MBForm();
	virtual ~MBForm();
	static MBForm*  CreateOne(CWnd* pParent);
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MBForm	};
#endif
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};

