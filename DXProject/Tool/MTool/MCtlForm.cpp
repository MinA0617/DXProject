#include "stdafx.h"
#include "MCtlForm.h"
#include "MTool.h"
#include "afxdialogex.h"

IMPLEMENT_DYNAMIC(MCtlForm, CFormView)

MCtlForm::MCtlForm() : CFormView(IDD_CtlForm)
{

}


MCtlForm::~MCtlForm()
{
}

void MCtlForm::DoDataExchange(CDataExchange * pDX)
{
	CFormView::DoDataExchange(pDX);
	//DDX_Text(pDX, IDC_GRAVITY_X, m_fGravityX);
	//DDX_Text(pDX, IDC_GRAVITY_Y, m_fGravityY);
	//DDX_Text(pDX, IDC_GRAVITY_Z, m_fGravityZ);
	//DDX_Text(pDX, IDC_VELOCITY_X, m_fVelocityX);
	//DDX_Text(pDX, IDC_VELOCITY_Y, m_fVelocityY);
	//DDX_Text(pDX, IDC_VELOCITY_Z, m_fVelocityZ);
}

void MCtlForm::OnBnClickedLod()
{
	UpdateData(TRUE);
	g_bIsLOD == true;
}

void MCtlForm::OnBnClickedSolid()
{
	UpdateData(TRUE);
	I_Device.m_RasterizerStateID = MWireFrame;
}

MCtlForm * MCtlForm::CreateOne(CWnd * pParent)
{
	MCtlForm* pForm = new MCtlForm;
	pForm->Create(NULL, NULL, WS_CHILD | WS_VISIBLE,
		CRect(0, 0, 500, 500), pParent, 0, NULL);
	return pForm;
}

BEGIN_MESSAGE_MAP(MCtlForm, CFormView)
//	ON_BN_CLICKED(IDOK, &MCtlForm::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON_LOD, &MCtlForm::OnBnClickedLod)
	ON_BN_CLICKED(IDC_BUTTON_SOLID, &MCtlForm::OnBnClickedSolid)
END_MESSAGE_MAP()
