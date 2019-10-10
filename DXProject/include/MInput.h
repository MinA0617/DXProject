//#pragma once
//#include "MStd.h"
//
////#define DIRECTINPUT_VERSION 0x0800
//
//class MInput : public MSingleton<MInput>
//{
//	friend class MSingleton<MInput>;
//private:
//	MInput();
//public:
//	virtual ~MInput();
//public:
//	bool Init();
//	bool Frame();
//	bool Render();
//	bool Release();
//public:
//	LPDIRECTINPUT8			m_pDirectInput;		// ���̷�Ʈ��ǲ ��ü
//	LPDIRECTINPUTDEVICE8	m_pDidevKeyBoard;	// Ű���� ��ü
//	LPDIRECTINPUTDEVICE8	m_pDidevMouse;		// ���콺 ��ü
//public:
//	DWORD					m_dwImmediate[MAX_PATH];
//	DIDEVICEOBJECTDATA		m_didod[256];
//
//
//	BYTE					m_KeyState[256];
//	DWORD					m_dwElements;
//	DWORD m_dwMouseState[3];
//	DWORD m_dwBeforeMouseState[3];
//	POINT m_Mousrpos;
//	LRESULT MsgProc(MSG msg); //// ���콺 �߰��
//	DWORD KeyCheck(DWORD dwkey);
//public:
//	DWORD KeyChange;
//};
//
//
//#define I_Input MInput::GetInstance()
#pragma once
#include "MStd.h"


class MInput : public MSingleton<MInput>
{
	friend class MSingleton<MInput>;
private:
	MInput();
public:
	virtual ~MInput();
public:
	bool Init();
	bool Frame();
	bool Render();
	bool Release();
public:
	DWORD m_dwKeyState[256];
	DWORD m_dwMouseState[3];
	DWORD m_dwBeforeMouseState[3];
	POINT m_Mousrpos;
	////DWORD Key(DWORD dwKey); 
	LRESULT MsgProc(MSG msg); //// ���콺 �߰��
	DWORD KeyCheck(DWORD dwkey);
public:
	DWORD KeyChange;
};


#define I_Input MInput::GetInstance()