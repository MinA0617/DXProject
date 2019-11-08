//#pragma once
//
//
//// MCreateMapDlg 대화 상자
//
//class MCreateMapDlg : public CDialogEx
//{
//	DECLARE_DYNAMIC(MCreateMapDlg)
//
//public:
//	MCreateMapDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
//	virtual ~MCreateMapDlg();
//
//// 대화 상자 데이터입니다.
//#ifdef AFX_DESIGN_TIME
//	enum { IDD = IDD_DIALOG1 };
//#endif
//
//protected:
//	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
//
//	DECLARE_MESSAGE_MAP()
//public:
//	// 지형 가로 정점 개수 기입
//	bool m_bIsNew;
//	afx_msg void OnEnChangeEdit1();
//	int m_fFiledX;
//	afx_msg void OnEnChangeEdit2();
//	int m_fFiledY;
//	afx_msg void OnBnClickedOk();
//	afx_msg void OnBnClickedRadio1();
//	afx_msg void OnBnClickedRadio2();
//	CString m_sFilename;
//	afx_msg void OnBnClickedFindFile();
//	afx_msg void OnBnClickedButton2();
//	static int iCountFiled;
//	int m_TileSize;
//};
