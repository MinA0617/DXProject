#include "stdafx.h"
#include "MObjectPane.h"
#include "MTool.h"
// TUIToolPane

IMPLEMENT_DYNAMIC(MObjectPane, CDockablePane)

MObjectPane::MObjectPane()
{

}

MObjectPane::~MObjectPane()
{
}


BEGIN_MESSAGE_MAP(MObjectPane, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_MOUSEACTIVATE()
END_MESSAGE_MAP()



// TUIToolPane 메시지 처리기




int MObjectPane::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.
	m_wndForm = MObjectCtlForm::CreateOne(this);
	return 0;
}


void MObjectPane::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);

	if (m_wndForm)
	{
		m_wndForm->SetWindowPos(NULL, 0, 0, cx, cy,
			SWP_NOZORDER);
	}
}


int MObjectPane::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message)
{
	CFrameWnd* pParentFrame = GetParentFrame();
	if (pParentFrame == pDesktopWnd ||
		pDesktopWnd->IsChild(pParentFrame))
	{
		return CDockablePane::OnMouseActivate(pDesktopWnd, nHitTest, message);
	}
	return MA_NOACTIVATE;
}
