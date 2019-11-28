// MObjectCtlForm.cpp: 구현 파일
//

#include "stdafx.h"
#include "MTool.h"
#include "MObjectCtlForm.h"
#include "afxdialogex.h"


// MObjectCtlForm 대화 상자

IMPLEMENT_DYNAMIC(MObjectCtlForm, CFormView)

MObjectCtlForm* MObjectCtlForm::CreateOne(CWnd * pParent)
{
	MObjectCtlForm* pForm = new MObjectCtlForm;
	BOOL result = pForm->Create(NULL, NULL, WS_CHILD | WS_VISIBLE, CRect(0, 0, 450, 800), pParent, 0, NULL);
	return pForm;
}

MObjectCtlForm::MObjectCtlForm() : CFormView(IDD_ObjectCtlForm)
, m_fRotationX(0)
, m_fRotationY(0)
, m_fRotationZ(0)
, m_fScale(1)
, m_iRotationVariationX(0)
, m_iRotationVariationY(0)
, m_iRotationVariationZ(0)
, m_iScaleVariation(0)
{
	m_iCount = 0;
	m_iOnObject = 0;
}

MObjectCtlForm::~MObjectCtlForm()
{
}

void MObjectCtlForm::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_OBJ_LIST, m_List);
	DDX_Text(pDX, IDD_Rotation_X, m_fRotationX);
	DDV_MinMaxFloat(pDX, m_fRotationX, 0, 360);
	DDX_Text(pDX, IDD_Rotation_Y, m_fRotationY);
	DDV_MinMaxFloat(pDX, m_fRotationY, 0, 360);
	DDX_Text(pDX, IDD_Rotation_Z, m_fRotationZ);
	DDV_MinMaxFloat(pDX, m_fRotationZ, 0, 360);
	DDX_Text(pDX, IDC_Scale, m_fScale);
	DDV_MinMaxFloat(pDX, m_fScale, 0, FLT_MAX);
	DDX_Control(pDX, IDC_ScaleSlider, m_ScaleVariationCtl);
	DDX_Control(pDX, IDC_RotationSliderX, m_RotationXVariationCtl);
	DDX_Control(pDX, IDC_RotationSliderY, m_RotationYVariationCtl);
	DDX_Control(pDX, IDC_RotationSliderZ, m_RotationZVariationCtl);
}

BEGIN_MESSAGE_MAP(MObjectCtlForm, CFormView)
	ON_BN_CLICKED(IDC_LOAD_BUTTON, &MObjectCtlForm::OnBnClickedLoadButton)
	ON_BN_CLICKED(IDC_DELETE_BUTTON, &MObjectCtlForm::OnBnClickedDeleteButton)
	ON_BN_CLICKED(IDC_CREATE_IN_WORLD_BUTTON, &MObjectCtlForm::OnBnClickedCreateInWorldButton)
	ON_BN_CLICKED(IDC_DELETE_IN_WORLD_BUTTON, &MObjectCtlForm::OnBnClickedDeleteInWorldButton)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_OBJ_LIST, &MObjectCtlForm::OnLvnItemchangedObjList)
	ON_EN_CHANGE(IDD_Rotation_X, &MObjectCtlForm::OnEnChangeRotationX)
	ON_EN_CHANGE(IDD_Rotation_Y, &MObjectCtlForm::OnEnChangeRotationY)
	ON_EN_CHANGE(IDD_Rotation_Z, &MObjectCtlForm::OnEnChangeRotationZ)
	ON_EN_CHANGE(IDC_Scale, &MObjectCtlForm::OnEnChangeScale)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_ScaleSlider, &MObjectCtlForm::OnNMCustomdrawScaleslider)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_RotationSliderX, &MObjectCtlForm::OnNMCustomdrawRotationsliderx)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_RotationSliderY, &MObjectCtlForm::OnNMCustomdrawRotationslidery)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_RotationSliderZ, &MObjectCtlForm::OnNMCustomdrawRotationsliderz)
END_MESSAGE_MAP()


// MObjectCtlForm 메시지 처리기


bool MObjectCtlForm::LoadOBJ(CString cstr)
{
	vector<M_STR> namelist;
	M_STR str = cstr;
	if (I_Parser.Load(str, namelist))
	{
		TCHAR Drive[MAX_PATH] = { 0, };
		TCHAR Dir[MAX_PATH] = { 0, };
		TCHAR Name[MAX_PATH] = { 0, };
		TCHAR Ext[MAX_PATH] = { 0, };
		_tsplitpath_s(str.c_str(), Drive, Dir, Name, Ext); // 패스와 이름과 확장자 끊어주기

		CString groupname = Name;
		m_List.InsertItem(m_iCount, groupname);
		m_List.SetItemText(m_iCount, 1, groupname);

		theApp.m_Tool.instance.filepath.push_back(str);
		vector<int> idlist;
		for (auto temp : namelist)
		{
			int id = I_3DObjectMgr.GetInstanceModelID(temp);
			if (id != -1)
			{
				idlist.push_back(id);
			}
		}
		theApp.m_Tool.instance.table.insert(make_pair(m_iCount, Name));
		theApp.m_Tool.instance.list.insert(make_pair(Name, idlist));
		m_iCount++;
		return true;
	}
	return false;
}

void MObjectCtlForm::OnBnClickedLoadButton()
{
	CString selFileName;
	CFileDialog dlg(TRUE, L"OBJ", NULL, OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST, L"OBJ Files(*.OBJ)|*.OBJ", this);
	if (dlg.DoModal() == IDOK)
	{
		selFileName = dlg.GetPathName();
		CMToolApp* pApp = (CMToolApp*)AfxGetApp();
		LoadOBJ(selFileName);
		return;
	}
}


void MObjectCtlForm::OnBnClickedDeleteButton()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void MObjectCtlForm::OnBnClickedCreateInWorldButton()
{
	theApp.m_Tool.m_State = CREATEOBJ;
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void MObjectCtlForm::OnBnClickedDeleteInWorldButton()
{
	theApp.m_Tool.m_State = DELETEOBJ;
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void MObjectCtlForm::OnLvnItemchangedObjList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_iOnObject = pNMLV->iItem;
	theApp.m_Tool.instance.m_iID = m_iOnObject;
	*pResult = 0;
}

void MObjectCtlForm::OnEnChangeRotationX()
{
	UpdateData(TRUE);
	theApp.m_Tool.instance.m_fRotationX = m_fRotationX;
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CFormView::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void MObjectCtlForm::OnEnChangeRotationY()
{
	UpdateData(TRUE);
	theApp.m_Tool.instance.m_fRotationY = m_fRotationY;
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CFormView::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void MObjectCtlForm::OnEnChangeRotationZ()
{
	UpdateData(TRUE);
	theApp.m_Tool.instance.m_fRotationZ = m_fRotationZ;
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CFormView::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void MObjectCtlForm::OnEnChangeScale()
{
	UpdateData(TRUE);
	theApp.m_Tool.instance.m_fScale = m_fScale;
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CFormView::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void MObjectCtlForm::OnNMCustomdrawScaleslider(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	if (m_ScaleVariationCtl)	m_iScaleVariation = m_ScaleVariationCtl.GetPos();
	theApp.m_Tool.instance.m_iScaleVariation = m_iScaleVariation;
	*pResult = 0;
}

void MObjectCtlForm::OnNMCustomdrawRotationsliderx(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	if(m_RotationXVariationCtl)	m_iRotationVariationX = m_RotationXVariationCtl.GetPos();
	theApp.m_Tool.instance.m_iRotationVariationX = m_iRotationVariationX;
	*pResult = 0;
}

void MObjectCtlForm::OnNMCustomdrawRotationslidery(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	if (m_RotationYVariationCtl)	m_iRotationVariationY = m_RotationYVariationCtl.GetPos();
	theApp.m_Tool.instance.m_iRotationVariationY = m_iRotationVariationY;
	*pResult = 0;
}


void MObjectCtlForm::OnNMCustomdrawRotationsliderz(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	if (m_RotationZVariationCtl)	m_iRotationVariationZ = m_RotationZVariationCtl.GetPos();
	theApp.m_Tool.instance.m_iRotationVariationZ = m_iRotationVariationZ;
	*pResult = 0;
}
