// MMapCtlForm.cpp: 구현 파일
//

#include "stdafx.h"
#include "MTool.h"
#include "MMapCtlForm.h"
#include "afxdialogex.h"


// MMapCtlForm 대화 상자

IMPLEMENT_DYNAMIC(MMapCtlForm, CFormView)

MMapCtlForm*  MMapCtlForm::CreateOne(CWnd* pParent)
{
	MMapCtlForm* pForm = new MMapCtlForm;
	pForm->Create(NULL, NULL, WS_CHILD | WS_VISIBLE, CRect(0, 0, 500, 800), pParent, 0, NULL);
	return pForm;
}

MMapCtlForm::MMapCtlForm() : CFormView(IDD_MapCtlForm)
, m_iBrushSize(100)
, m_iBrushOpacity(100)
, m_iPushSize(100)
, m_iPushPower(100)
{
	m_iTileID = 0;
	m_iBrushID = 0;
	m_iLayer = 0;
	m_iTileCount = 0;
	m_iBrushCount = 0;
}

MMapCtlForm::~MMapCtlForm()
{
}

void MMapCtlForm::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_BRUSH_SIZE, m_iBrushSize);
	DDV_MinMaxInt(pDX, m_iBrushSize, 1, 999999);
	DDX_Text(pDX, IDC_BRUSH_OPACITY, m_iBrushOpacity);
	DDV_MinMaxInt(pDX, m_iBrushOpacity, 0, 100);
	DDX_Text(pDX, IDC_PUSH_SIZE, m_iPushSize);
	DDV_MinMaxInt(pDX, m_iPushSize, 0, 999999);
	DDX_Text(pDX, IDC_PUSH_POWER, m_iPushPower);
	DDX_Control(pDX, IDC_TILE_LIST, m_TileLIst);
	DDX_Control(pDX, IDC_BRUSH_LIST, m_BrushList);
	DDX_Control(pDX, IDC_BUTTON_TILE, m_TileBotton);
	DDX_Control(pDX, IDC_BUTTON_BRUSH, m_DrawBotton);
	DDX_Control(pDX, IDC_BUTTON_PUSH, m_PushBotton);
}


BEGIN_MESSAGE_MAP(MMapCtlForm, CFormView)
	ON_BN_CLICKED(IDC_RADIO_LAYER1, &MMapCtlForm::OnBnClickedRadioLayer1)
	ON_BN_CLICKED(IDC_RADIO_LAYER2, &MMapCtlForm::OnBnClickedRadioLayer2)
	ON_BN_CLICKED(IDC_RADIO_LAYER3, &MMapCtlForm::OnBnClickedRadioLayer3)
	ON_NOTIFY(UDN_DELTAPOS, IDC_BRUSH_SIZE_SPIN, &MMapCtlForm::OnDeltaposBrushSize)
	ON_NOTIFY(UDN_DELTAPOS, IDC_BRUSH_OPACITY_SPIN, &MMapCtlForm::OnDeltaposBrushOpacity)
	ON_NOTIFY(UDN_DELTAPOS, IDC_PUSH_SIZE_SPIN, &MMapCtlForm::OnDeltaposPushSize)
	ON_NOTIFY(UDN_DELTAPOS, IDC_PUSH_POWER_SPIN, &MMapCtlForm::OnDeltaposPushPower)
	ON_EN_CHANGE(IDC_BRUSH_SIZE, &MMapCtlForm::OnEnChangeBrushSize)
	ON_EN_CHANGE(IDC_BRUSH_OPACITY, &MMapCtlForm::OnEnChangeBrushOpacity)
	ON_EN_CHANGE(IDC_PUSH_SIZE, &MMapCtlForm::OnEnChangePushSize)
	ON_EN_CHANGE(IDC_PUSH_POWER, &MMapCtlForm::OnEnChangePushPower)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_BRUSH_LIST, &MMapCtlForm::OnLvnItemchangedBrushList)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_TILE_LIST, &MMapCtlForm::OnLvnItemchangedTileList)
	ON_BN_CLICKED(IDC_OPEN_TILE, &MMapCtlForm::OnBnClickedOpenTile)
	ON_BN_CLICKED(IDC_OPEN_BRUSH, &MMapCtlForm::OnBnClickedOpenBrush)
	ON_BN_CLICKED(IDC_BUTTON_TILE, &MMapCtlForm::OnBnClickedButtonTile)
	ON_BN_CLICKED(IDC_BUTTON_BRUSH, &MMapCtlForm::OnBnClickedButtonBrush)
	ON_BN_CLICKED(IDC_BUTTON_PUSH, &MMapCtlForm::OnBnClickedButtonPush)
END_MESSAGE_MAP()


// MMapCtlForm 메시지 처리기


void MMapCtlForm::OnBnClickedRadioLayer1()
{
	m_iLayer = 0;
	theApp.m_Tool.canvas.m_Channel = m_iLayer;
	theApp.m_Tool.tileing.m_iCurLayer = m_iLayer;
}


void MMapCtlForm::OnBnClickedRadioLayer2()
{
	m_iLayer = 1;
	theApp.m_Tool.canvas.m_Channel = m_iLayer;
	theApp.m_Tool.tileing.m_iCurLayer = m_iLayer;
}


void MMapCtlForm::OnBnClickedRadioLayer3()
{
	m_iLayer = 2;
	theApp.m_Tool.canvas.m_Channel = m_iLayer;
	theApp.m_Tool.tileing.m_iCurLayer = m_iLayer;
}


void MMapCtlForm::OnDeltaposBrushSize(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (pNMUpDown->iDelta < 0) m_iBrushSize++;
	else m_iBrushSize--;
	if (m_iBrushSize < 1) m_iBrushSize = 1;
	UpdateData(FALSE);
	*pResult = 0;

	theApp.m_Tool.canvas.m_iRadius = m_iBrushSize;
}


void MMapCtlForm::OnDeltaposBrushOpacity(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (pNMUpDown->iDelta < 0) m_iBrushOpacity++;
	else m_iBrushOpacity--;
	if (m_iBrushOpacity < 0) m_iBrushOpacity = 0;
	if (m_iBrushOpacity > 100) m_iBrushOpacity = 100;
	UpdateData(FALSE);
	*pResult = 0;

	theApp.m_Tool.canvas.m_fOpacity = m_iBrushOpacity / 200.0 + 0.5;
}


void MMapCtlForm::OnDeltaposPushSize(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (pNMUpDown->iDelta < 0) m_iPushSize++;
	else m_iPushSize--;
	if (m_iPushSize < 0) m_iPushSize = 0;
	UpdateData(FALSE);
	*pResult = 0;

	theApp.m_Tool.pushpull.SetRadius(m_iPushSize);
}


void MMapCtlForm::OnDeltaposPushPower(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (pNMUpDown->iDelta < 0) m_iPushPower++;
	else m_iPushPower--;
	UpdateData(FALSE);
	*pResult = 0;

	theApp.m_Tool.pushpull.SetPower(m_iPushPower);
}


void MMapCtlForm::OnEnChangeBrushSize()
{
	UpdateData(TRUE);
	theApp.m_Tool.canvas.m_iRadius = m_iBrushSize;
}


void MMapCtlForm::OnEnChangeBrushOpacity()
{
	UpdateData(TRUE);
	theApp.m_Tool.canvas.m_fOpacity = m_iBrushOpacity / 200.0 + 0.5;;
}


void MMapCtlForm::OnEnChangePushSize()
{
	UpdateData(TRUE);
	theApp.m_Tool.pushpull.SetRadius(m_iPushSize);
}


void MMapCtlForm::OnEnChangePushPower()
{
	UpdateData(TRUE);
	theApp.m_Tool.pushpull.SetPower(m_iPushPower);
}


void MMapCtlForm::OnLvnItemchangedBrushList(NMHDR *pNMHDR, LRESULT *pResult)
{
	UpdateData(TRUE);
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_iBrushID = pNMLV->iItem;
	theApp.m_Tool.canvas.m_BrushID = m_iBrushID;
	*pResult = 0;
}


void MMapCtlForm::OnLvnItemchangedTileList(NMHDR *pNMHDR, LRESULT *pResult)
{
	UpdateData(TRUE);
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_iTileID = pNMLV->iItem;
	theApp.m_Tool.tileing.m_iTextureID = m_iTileID;
	*pResult = 0;
}


void MMapCtlForm::OnBnClickedOpenTile()
{
	if (I_3DObjectMgr.m_InWorldFiled == nullptr)
	{
		AfxMessageBox(L"실패! 지형이 생성되지 않았습니다.");
		return;
	}
	CString selFileName;
	CFileDialog dlg(TRUE, L"bmp", NULL, OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST, L"bmp Files(*.bmp)|*.bmp|png Files(*.png) | *.png | All Files(*.*)|*.*|", this);
	if (dlg.DoModal() == IDOK)
	{
		selFileName = dlg.GetPathName();
		CMToolApp* pApp = (CMToolApp*)AfxGetApp();
		M_STR str = selFileName;
		if (I_3DObjectMgr.m_InWorldFiled->ground->Load_MAP(str))
		{
			TCHAR Drive[MAX_PATH] = { 0, };
			TCHAR Dir[MAX_PATH] = { 0, };
			TCHAR Name[MAX_PATH] = { 0, };
			TCHAR Ext[MAX_PATH] = { 0, };
			_tsplitpath_s(selFileName, Drive, Dir, Name, Ext); // 패스와 이름과 확장자 끊어주기
			CString tilename = Name; // 이름저장

			m_TileLIst.InsertItem(m_iTileCount, tilename);
			m_TileLIst.SetItemText(m_iTileCount, 1, tilename);
			m_iTileCount++;
		}
		else
		{
			AfxMessageBox(L"실패! 올파른 파일이 아닙다.");
			return;
		}

	}
}


void MMapCtlForm::OnBnClickedOpenBrush()
{
	if (I_3DObjectMgr.m_InWorldFiled == nullptr)
	{
		AfxMessageBox(L"실패! 지형이 생성되지 않았습니다.");
		return;
	}
	CString selFileName;
	CFileDialog dlg(TRUE, L"bmp", NULL, OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST, L"bmp Files(*.bmp)|*.bmp|png Files(*.png) | *.png | All Files(*.*)|*.*|", this);
	if (dlg.DoModal() == IDOK)
	{
		selFileName = dlg.GetPathName();
		CMToolApp* pApp = (CMToolApp*)AfxGetApp();
		M_STR str = selFileName;
		if (theApp.m_Tool.canvas.LoadBrush(str))
		{
			TCHAR Drive[MAX_PATH] = { 0, };
			TCHAR Dir[MAX_PATH] = { 0, };
			TCHAR Name[MAX_PATH] = { 0, };
			TCHAR Ext[MAX_PATH] = { 0, };
			_tsplitpath_s(selFileName, Drive, Dir, Name, Ext); // 패스와 이름과 확장자 끊어주기
			CString tilename = Name; // 이름저장

			//CDC* p;
			//p.getb
			//CBitmap temp;
			//CImageList m_ImageList;
			//m_BrushList.CreateDragImage(m_iBrushCount, );
			m_BrushList.InsertItem(m_iBrushCount, tilename);
			m_BrushList.SetItemText(m_iBrushCount, 1, tilename);
			m_iBrushCount++;
		}
		else
		{
			AfxMessageBox(L"실패! 올파른 파일이 아닙다.");
		}
	}
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void MMapCtlForm::OnBnClickedButtonTile()
{
	UpdateData(TRUE);
	bool istrue = m_TileBotton.GetState();
	m_DrawBotton.SetState(false);
	m_PushBotton.SetState(false);
	if (istrue)	theApp.m_Tool.m_State = TILEING;
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void MMapCtlForm::OnBnClickedButtonBrush()
{
	UpdateData(TRUE);
	bool istrue = m_DrawBotton.GetState();
	m_TileBotton.SetState(false);
	m_PushBotton.SetState(false);
	if (istrue)	theApp.m_Tool.m_State = BRUSH;
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void MMapCtlForm::OnBnClickedButtonPush()
{
	UpdateData(TRUE);
	bool istrue = m_PushBotton.GetState();
	m_TileBotton.SetState(false);
	m_DrawBotton.SetState(false);
	if (istrue)	theApp.m_Tool.m_State = PUSH;
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}
