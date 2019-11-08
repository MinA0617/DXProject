// Sam.cpp: 구현 파일
//

#include "stdafx.h"
#include "BTool.h"
#include "Sam.h"
#include "afxdialogex.h"


// Sam 대화 상자

IMPLEMENT_DYNAMIC(Sam, CDialog)

Sam::Sam(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_DIALOGBAR, pParent)
{

}

Sam::~Sam()
{
}

void Sam::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(Sam, CDialog)
END_MESSAGE_MAP()


// Sam 메시지 처리기
