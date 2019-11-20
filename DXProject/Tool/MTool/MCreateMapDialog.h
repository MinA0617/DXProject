#pragma once
#include "MStd.h"

// MCreateMapDialog 대화 상자

class MCreateMapDialog : public CDialogEx
{
	DECLARE_DYNAMIC(MCreateMapDialog)

public:
	MCreateMapDialog(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~MCreateMapDialog();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_CREATE_MAP };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	static int iCountFiled;
	M_STR m_FileName;
	BOOL m_Radio_Select;
	int m_iTileSize;
	float m_fLeafSize;
	int m_ctMapCount;
	float m_ctMapHeight;
	CButton m_FindFile;
	bool	m_bIsNew;
	afx_msg void OnBnClickedRadioNew();
	afx_msg void OnBnClickedRadioLoad();
	afx_msg void OnEnChangeMapCount();
	afx_msg void OnEnChangeMapHeight();
	afx_msg void OnEnChangeMapLeafsize();
	afx_msg void OnEnChangeMapTilesize();
	afx_msg void OnBnClickedCreate();
	afx_msg void OnBnClickedLoadFile();
	afx_msg void OnEnChangeMapAlphatexturesize();
	int m_iTextureSize;
};
