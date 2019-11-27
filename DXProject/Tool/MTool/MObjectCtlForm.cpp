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
	BOOL result = pForm->Create(NULL, NULL, WS_CHILD | WS_VISIBLE, CRect(0, 0, 300, 800), pParent, 0, NULL);
	return pForm;
}

MObjectCtlForm::MObjectCtlForm() : CFormView(IDD_ObjectCtlForm)
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
}

BEGIN_MESSAGE_MAP(MObjectCtlForm, CFormView)
	ON_BN_CLICKED(IDC_LOAD_BUTTON, &MObjectCtlForm::OnBnClickedLoadButton)
	ON_BN_CLICKED(IDC_DELETE_BUTTON, &MObjectCtlForm::OnBnClickedDeleteButton)
	ON_BN_CLICKED(IDC_CREATE_IN_WORLD_BUTTON, &MObjectCtlForm::OnBnClickedCreateInWorldButton)
	ON_BN_CLICKED(IDC_DELETE_IN_WORLD_BUTTON, &MObjectCtlForm::OnBnClickedDeleteInWorldButton)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_OBJ_LIST, &MObjectCtlForm::OnLvnItemchangedObjList)
END_MESSAGE_MAP()


// MObjectCtlForm 메시지 처리기


void MObjectCtlForm::OnBnClickedLoadButton()
{
	CString selFileName;
	CFileDialog dlg(TRUE, L"OBJ", NULL, OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST, L"OBJ Files(*.OBJ)|*.OBJ", this);
	if (dlg.DoModal() == IDOK)
	{
		selFileName = dlg.GetPathName();
		CMToolApp* pApp = (CMToolApp*)AfxGetApp();
		vector<M_STR> namelist;
		M_STR str = selFileName;
		if (I_Parser.Load(str, namelist))
		{
			for (auto temp : namelist)
			{
				CString name = temp.c_str(); // 이름저장
				m_List.InsertItem(m_iCount, name);
				m_List.SetItemText(m_iCount, 1, name);
				m_iCount++;
			}
		}
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
