#include "stdafx.h"
#include "MBForm.h"
#include "BTool.h"
#include "afxdialogex.h"

IMPLEMENT_DYNAMIC(MBForm, CFormView)

MBForm*  MBForm::CreateOne(CWnd* pParent)
{
	MBForm* pForm = new MBForm;
	pForm->Create(NULL, NULL, WS_CHILD | WS_VISIBLE,
		CRect(0, 0, 500, 500), pParent, 0, NULL);
	return pForm;
}

void MBForm::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

MBForm::MBForm() : CFormView(IDD_MBForm)
{
}


MBForm::~MBForm()
{
}

BEGIN_MESSAGE_MAP(MBForm, CFormView)
END_MESSAGE_MAP()
