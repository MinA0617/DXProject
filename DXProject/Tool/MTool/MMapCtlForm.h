#pragma once


// MMapCtlForm 대화 상자
class MMapCtlForm : public CFormView
{
	DECLARE_DYNAMIC(MMapCtlForm)

public:
	MMapCtlForm();   // 표준 생성자입니다.
	virtual ~MMapCtlForm();
	static MMapCtlForm*  CreateOne(CWnd* pParent);
public:
	bool LoadBrush(CString path);
	bool LoadTile(CString path);
// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MapCtlForm };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedRadioLayer1();
	afx_msg void OnBnClickedRadioLayer2();
	afx_msg void OnBnClickedRadioLayer3();
public:
	afx_msg void OnDeltaposBrushSize(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposBrushOpacity(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposPushSize(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposPushPower(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEnChangeBrushSize();
	afx_msg void OnEnChangeBrushOpacity();
	afx_msg void OnEnChangePushSize();
	afx_msg void OnEnChangePushPower();
	afx_msg void OnLvnItemchangedBrushList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnItemchangedTileList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedOpenTile();
	afx_msg void OnBnClickedOpenBrush();
	afx_msg void OnBnClickedButtonTile();
	afx_msg void OnBnClickedButtonBrush();
	afx_msg void OnBnClickedButtonPush();

	int			m_iLayer;
	int			m_iBrushSize;
	int			m_iBrushOpacity;
	float		m_iPushSize;
	float		m_iPushPower;
	CListCtrl	m_TileLIst;
	CListCtrl	m_BrushList;
	int			m_iTileID;
	int			m_iBrushID;
	int			m_iTileCount;
	int			m_iBrushCount;
	CButton		m_TileBotton;
	CButton		m_DrawBotton;
	CButton		m_PushBotton;
};
