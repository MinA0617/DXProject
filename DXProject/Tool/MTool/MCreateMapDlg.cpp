//// MCreateMapDlg.cpp: 구현 파일
////
//
#include "stdafx.h"
//#include "MTool.h"
//#include "MCreateMapDlg.h"
//#include "afxdialogex.h"
//
//
//// MCreateMapDlg 대화 상자
//
//IMPLEMENT_DYNAMIC(MCreateMapDlg, CDialogEx)
//
//int MCreateMapDlg::iCountFiled = 0;
//
//MCreateMapDlg::MCreateMapDlg(CWnd* pParent /*=nullptr*/)
//	: CDialogEx(IDD_DIALOG1, pParent)
//	, m_fFiledX(1)
//	, m_fFiledY(1)
//	, m_sFilename(_T(""))
//	, m_TileSize(0)
//{
//
//}
//
//
//MCreateMapDlg::~MCreateMapDlg()
//{
//}
//
//void MCreateMapDlg::DoDataExchange(CDataExchange* pDX)
//{
//	CDialogEx::DoDataExchange(pDX);
//	//DDX_Text(pDX, IDC_EDIT1, m_strRowCount);
//	//DDX_Control(pDX, IDC_EDIT2, m_ctlColCount);
//	//DDX_Control(pDX, IDC_COMBO1, m_ctlCellSize);
//	//DDX_CBString(pDX, IDC_COMBO2, m_iText);
//	DDX_Text(pDX, IDC_EDIT1, m_fFiledX);
//	DDV_MinMaxInt(pDX, m_fFiledX, 1, 16385);
//	DDX_Text(pDX, IDC_EDIT2, m_fFiledY);
//	DDV_MinMaxInt(pDX, m_fFiledY, 1, 16384);
//	DDX_Text(pDX, IDC_EDIT3, m_TileSize);
//	DDV_MinMaxInt(pDX, m_TileSize, 1, 1024);
//}
//
//
//BEGIN_MESSAGE_MAP(MCreateMapDlg, CDialogEx)
//	//ON_EN_CHANGE(IDC_EDIT1, &MCreateMapDlg::OnEnChangeEdit1)
//	ON_BN_CLICKED(IDOK, &MCreateMapDlg::OnBnClickedOk)
//	ON_EN_CHANGE(IDC_EDIT1, &MCreateMapDlg::OnEnChangeEdit1)
//	ON_EN_CHANGE(IDC_EDIT2, &MCreateMapDlg::OnEnChangeEdit2)
//	ON_BN_CLICKED(IDC_RADIO6, &MCreateMapDlg::OnBnClickedRadio1)
//	ON_BN_CLICKED(IDC_RADIO7, &MCreateMapDlg::OnBnClickedRadio2)
//	ON_BN_CLICKED(IDC_BUTTON1, &MCreateMapDlg::OnBnClickedFindFile)
//	ON_BN_CLICKED(IDC_BUTTON2, &MCreateMapDlg::OnBnClickedButton2)
//END_MESSAGE_MAP()
//
//
//// MCreateMapDlg 메시지 처리기
//
//
//void MCreateMapDlg::OnEnChangeEdit1()
//{
//	UpdateData(TRUE);
//}
//
//void MCreateMapDlg::OnEnChangeEdit2()
//{
//	UpdateData(TRUE);
//}
//
//void MCreateMapDlg::OnBnClickedOk()
//{
//	UpdateData(TRUE);
//	if (m_bIsNew)
//	{
//		M_STR name = L"Filed";
//		wchar_t buffer[5];
//		_itow_s(iCountFiled++, buffer, 10);
//		name += buffer;
//		if (I_3DObjectMgr.CreateFiled(name, m_fFiledX))
//		{
//			CDialogEx::OnOK();
//			AfxMessageBox(L"성공! 축하한다");
//		}
//		else
//		{
//			CDialogEx::OnCancel();
//			AfxMessageBox(L"실패! 2의 배수 + 1 값으로 설정해라");
//		}
//	}
//	else
//	{
//		MParser ps;
//		if (ps.Load_HM(m_sFilename.GetString(), 10))
//		{
//			AfxMessageBox(L"성공! 축하한다");
//		}
//		else
//		{
//			AfxMessageBox(L"실패! 나는 원한다 올바른 파일");
//		}
//	}
//}
//
//void MCreateMapDlg::OnBnClickedRadio1()
//{
//	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
//	UpdateData(TRUE);
//	m_bIsNew = true;
//}
//
//void MCreateMapDlg::OnBnClickedRadio2()
//{
//	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
//	UpdateData(TRUE);
//	m_bIsNew = false;
//}
//
//void MCreateMapDlg::OnBnClickedFindFile()
//{
//	CString selFileName;
//	CFileDialog dlg(TRUE, L"bmp", NULL, OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST, L"bmp Files(*.bmp)|*.bmp|png Files(*.png) | *.png | All Files(*.*)|*.*|", this);
//	if (dlg.DoModal() == IDOK)
//	{
//		selFileName = dlg.GetPathName();
//		//selFileName += dlg.GetFileName();		
//		//AfxMessageBox(selFileName);
//		CMToolApp* pApp = (CMToolApp*)AfxGetApp();
//		m_sFilename = selFileName.GetBuffer();
//		//MParser ps;
//		//ps.Load_HM(selFileName.GetBuffer(), 10);
//	}
//	UpdateData(TRUE);
//}
//
//
//void MCreateMapDlg::OnBnClickedButton2()
//{
//	UpdateData(TRUE);
//	if (I_3DObjectMgr.m_InWorldFiled)
//	{
//		CString selFileName;
//		CFileDialog dlg(TRUE, L"bmp", NULL, OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST, L"bmp Files(*.bmp)|*.bmp|png Files(*.png) | *.png | All Files(*.*)|*.*|", this);
//
//		if (dlg.DoModal() == IDOK)
//		{
//			selFileName = dlg.GetPathName();
//			//selFileName += dlg.GetFileName();		
//			//AfxMessageBox(selFileName);
//			CMToolApp* pApp = (CMToolApp*)AfxGetApp();
//			m_sFilename = selFileName.GetBuffer();
//
//			if (I_3DObjectMgr.m_InWorldFiled->ground->Load_MAP(m_sFilename.GetString(), DIFFUSE))
//			{
//				AfxMessageBox(L"성공! 축하한다");
//			}
//			else
//			{
//				AfxMessageBox(L"실패! 올바른 파일이 아닙니다.");
//			}
//		}
//	}
//	else
//	{
//		AfxMessageBox(L"실패! 생성된 지형이 없습니다.");
//	}
//}
