// MCreateMapDialog.cpp: 구현 파일
//

#include "stdafx.h"
#include "MTool.h"
#include "MCreateMapDialog.h"
#include "afxdialogex.h"

// MCreateMapDialog 대화 상자

int MCreateMapDialog::iCountFiled = 0;

IMPLEMENT_DYNAMIC(MCreateMapDialog, CDialogEx)

M_STR m_FileName;
BOOL m_Radio_Select;
int m_iTileSize;
float m_fLeafSize;
int m_ctMapCount;
float m_ctMapHeight;
CButton m_FindFile;



MCreateMapDialog::MCreateMapDialog(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_CREATE_MAP, pParent)
	, m_Radio_Select(TRUE)
	, m_iTileSize(32)
	, m_fLeafSize(10)
	, m_ctMapCount(129)
	, m_ctMapHeight(100)
{

}

MCreateMapDialog::~MCreateMapDialog()
{
}

void MCreateMapDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_Map_TileSize, m_iTileSize);
	DDV_MinMaxInt(pDX, m_iTileSize, 2, 1024);
	DDX_Text(pDX, IDC_Map_LeafSize, m_fLeafSize);
	DDV_MinMaxFloat(pDX, m_fLeafSize, 0.01, 9999.00);
	DDX_Text(pDX, IDC_Map_Count, m_ctMapCount);
	DDV_MinMaxInt(pDX, m_iTileSize, 1, 16385);
	DDX_Text(pDX, IDC_Map_Height, m_ctMapHeight);
	DDV_MinMaxFloat(pDX, m_fLeafSize, 0.01, 9999.00);
	DDX_Control(pDX, IDC_BUTTON1, m_FindFile);
}


BEGIN_MESSAGE_MAP(MCreateMapDialog, CDialogEx)
	ON_BN_CLICKED(IDC_RADIO_MAP01, &MCreateMapDialog::OnBnClickedRadioNew)
	ON_BN_CLICKED(IDC_RADIO_MAP02, &MCreateMapDialog::OnBnClickedRadioLoad)
	ON_EN_CHANGE(IDC_Map_Count, &MCreateMapDialog::OnEnChangeMapCount)
	ON_EN_CHANGE(IDC_Map_Height, &MCreateMapDialog::OnEnChangeMapHeight)
	ON_EN_CHANGE(IDC_Map_LeafSize, &MCreateMapDialog::OnEnChangeMapLeafsize)
	ON_EN_CHANGE(IDC_Map_TileSize, &MCreateMapDialog::OnEnChangeMapTilesize)
	ON_BN_CLICKED(IDOK2, &MCreateMapDialog::OnBnClickedCreate)
	ON_BN_CLICKED(IDC_BUTTON1, &MCreateMapDialog::OnBnClickedLoadFile)
END_MESSAGE_MAP()

void MCreateMapDialog::OnBnClickedRadioNew()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int check = m_FindFile.GetCheck();
	m_bIsNew = true;
	UpdateData(TRUE);
}


void MCreateMapDialog::OnBnClickedRadioLoad()
{
	int check = m_FindFile.GetCheck();
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_bIsNew = false;
	UpdateData(TRUE);
}


void MCreateMapDialog::OnEnChangeMapCount()
{
	UpdateData(TRUE);
}


void MCreateMapDialog::OnEnChangeMapHeight()
{
	UpdateData(TRUE);
}


void MCreateMapDialog::OnEnChangeMapLeafsize()
{
	UpdateData(TRUE);
}


void MCreateMapDialog::OnEnChangeMapTilesize()
{
	UpdateData(TRUE);
}

void MCreateMapDialog::OnBnClickedLoadFile()
{
	CString selFileName;
	CFileDialog dlg(TRUE, L"bmp", NULL, OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST, L"bmp Files(*.bmp)|*.bmp|png Files(*.png) | *.png | All Files(*.*)|*.*|", this);
	if (dlg.DoModal() == IDOK)
	{
		selFileName = dlg.GetPathName();
		CMToolApp* pApp = (CMToolApp*)AfxGetApp();
		m_FileName = selFileName.GetBuffer();
	}
	UpdateData(TRUE);
}

void MCreateMapDialog::OnBnClickedCreate()
{
	// 생성버튼
	UpdateData(TRUE);
	if (I_3DObjectMgr.m_InWorldFiled)
	{
		AfxMessageBox(L"이미 생성된 지형이 있습니다. 새로 만드시겠습니까?", MB_OKCANCEL);

	}
	if (m_bIsNew)
	{
		M_STR name = L"Filed";
		wchar_t buffer[5];
		_itow_s(iCountFiled++, buffer, 10);
		name += buffer;
		if (I_3DObjectMgr.CreateFiled(name, m_ctMapCount, m_fLeafSize, m_iTileSize))
		{
			CDialogEx::OnOK();
			AfxMessageBox(L"성공하였습니다!");
		}
		else
		{
			CDialogEx::OnCancel();
			AfxMessageBox(L"실패하였습니다! 다음 항목을 확인해주시길 바랍니다. \nCount : 2의 배수 + 1 값\nTileSize : Count보다 작은 2의 배수");
		}
	}
	else
	{
		MParser ps;
		if (ps.Load_HM(m_FileName, m_fLeafSize, m_ctMapHeight, m_iTileSize))
		{
			AfxMessageBox(L"성공! 불러오기에 성공하였습니다.");
		}
		else
		{
			AfxMessageBox(L"실패! 올파른 파일이 아닙다.");
		}
	}
}