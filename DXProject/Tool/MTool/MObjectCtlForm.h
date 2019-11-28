#pragma once

// MObjectCtlForm 대화 상자

class MObjectCtlForm : public CFormView
{
	DECLARE_DYNAMIC(MObjectCtlForm)

public:
	MObjectCtlForm();   // 표준 생성자입니다.
	virtual ~MObjectCtlForm();
	static MObjectCtlForm*  CreateOne(CWnd* pParent);

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ObjectCtlForm };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	bool		 LoadOBJ(CString str);
	afx_msg void OnBnClickedLoadButton();
	afx_msg void OnBnClickedDeleteButton();
	afx_msg void OnBnClickedCreateInWorldButton();
	afx_msg void OnBnClickedDeleteInWorldButton();
	afx_msg void OnLvnItemchangedObjList(NMHDR *pNMHDR, LRESULT *pResult);

	CListCtrl m_List;
	int m_iOnObject;
	int	m_iCount;
	float m_fRotationX;
	float m_fRotationY;
	float m_fRotationZ;
	float m_fScale;
	int m_iRotationVariationX;
	int m_iRotationVariationY;
	int m_iRotationVariationZ;
	int m_iScaleVariation;
	afx_msg void OnEnChangeRotationX();
	afx_msg void OnEnChangeRotationY();
	afx_msg void OnEnChangeRotationZ();
	afx_msg void OnEnChangeScale();
	CSliderCtrl m_ScaleVariationCtl;
	CSliderCtrl m_RotationXVariationCtl;
	CSliderCtrl m_RotationYVariationCtl;
	CSliderCtrl m_RotationZVariationCtl;
	afx_msg void OnNMCustomdrawScaleslider(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawRotationsliderx(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawRotationslidery(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawRotationsliderz(NMHDR *pNMHDR, LRESULT *pResult);
};
