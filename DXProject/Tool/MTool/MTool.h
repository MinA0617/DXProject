﻿
// MTool.h: MTool 응용 프로그램의 기본 헤더 파일
//
#pragma once

#ifndef __AFXWIN_H__
	#error "PCH에 대해 이 파일을 포함하기 전에 'stdafx.h'를 포함합니다."
#endif

#include "Resource.h"       // 주 기호입니다.
#include "MToolCore.h"

// CMToolApp:
// 이 클래스의 구현에 대해서는 MTool.cpp을(를) 참조하세요.
//

class CMToolApp : public CWinAppEx
{
public:
	CMToolApp() noexcept;
	MToolCore	m_Tool;

// 재정의입니다.
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// 구현입니다.
	UINT  m_nAppLook;
	BOOL  m_bHiColorIcons;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();

	virtual BOOL OnIdle(LONG lCount);

	DECLARE_MESSAGE_MAP()
};

extern CMToolApp theApp;
