#include "MInput.h"

Keyinput g_ActionInput;
D3DXVECTOR2 g_MousePos;

MInput::MInput()
{
}


MInput::~MInput()
{
}

bool MInput::Init()
{
	return 0;
}

bool MInput::Frame()
{
	////////////////////키보드체크////////////////////
	g_ActionInput.F1  = KeyCheck(VK_F1 );
	g_ActionInput.F2  = KeyCheck(VK_F2 );
	g_ActionInput.F3  = KeyCheck(VK_F3 );
	g_ActionInput.F4  = KeyCheck(VK_F4 );
	g_ActionInput.F5  = KeyCheck(VK_F5 );
	g_ActionInput.F6  = KeyCheck(VK_F6 );
	g_ActionInput.F7  = KeyCheck(VK_F7 );
	g_ActionInput.F8  = KeyCheck(VK_F8 );
	g_ActionInput.F9  = KeyCheck(VK_F9 );
	g_ActionInput.F10 = KeyCheck(VK_F10);
	g_ActionInput.F11 = KeyCheck(VK_F11);
	g_ActionInput.F12 = KeyCheck(VK_F12);

	g_ActionInput.Num1 = KeyCheck(VK_NUMPAD1);
	g_ActionInput.Num2 = KeyCheck(VK_NUMPAD2);
	g_ActionInput.Num3 = KeyCheck(VK_NUMPAD3);
	g_ActionInput.Num4 = KeyCheck(VK_NUMPAD4);
	g_ActionInput.Num5 = KeyCheck(VK_NUMPAD5);
	g_ActionInput.Num6 = KeyCheck(VK_NUMPAD6);

	g_ActionInput.Insert	= KeyCheck(VK_INSERT);
	g_ActionInput.Home		= KeyCheck(VK_HOME);
	g_ActionInput.PgUp		= KeyCheck(VK_PRIOR);
	g_ActionInput.Delete	= KeyCheck(VK_DELETE);
	g_ActionInput.End		= KeyCheck(VK_END);
	g_ActionInput.PgDn		= KeyCheck(VK_NEXT);

	g_ActionInput.Q = KeyCheck('Q');
	g_ActionInput.W = KeyCheck('W');
	g_ActionInput.E = KeyCheck('E');
	g_ActionInput.R = KeyCheck('R');
	g_ActionInput.T = KeyCheck('T');
	g_ActionInput.Y = KeyCheck('Y');

	g_ActionInput.A = KeyCheck('A');
	g_ActionInput.S = KeyCheck('S');
	g_ActionInput.D = KeyCheck('D');
	g_ActionInput.F = KeyCheck('F');
	g_ActionInput.G = KeyCheck('G');
	g_ActionInput.H = KeyCheck('H');

	g_ActionInput.Z = KeyCheck('Z');
	g_ActionInput.X = KeyCheck('X');
	g_ActionInput.C = KeyCheck('C');
	g_ActionInput.V = KeyCheck('V');
	g_ActionInput.B = KeyCheck('B');
	g_ActionInput.N = KeyCheck('N');

	g_ActionInput.UP = KeyCheck(VK_UP);
	g_ActionInput.DOWN = KeyCheck(VK_DOWN);
	g_ActionInput.LEFT = KeyCheck(VK_LEFT);
	g_ActionInput.RIGHT = KeyCheck(VK_RIGHT);
	g_ActionInput.SPACE = KeyCheck(VK_SPACE);
	g_ActionInput.ENTER = KeyCheck(VK_RETURN);


	///////////////////마우스 체크////////////////////
	GetCursorPos(&m_Mousrpos);
	ScreenToClient(g_hWnd, &m_Mousrpos); 
	g_MousePos.x = (m_Mousrpos.x * 1) - g_rtWindowClient.right / 2;
	g_MousePos.y = (m_Mousrpos.y * -1) + g_rtWindowClient.bottom / 2;

	for (int iButton = 0; iButton < 3; iButton++)
	{
		if (m_dwBeforeMouseState[iButton] == KEY_PUSH)
		{
			if (m_dwMouseState[iButton] == KEY_PUSH)
			{
				m_dwMouseState[iButton] = KEY_HOLD;
			}
		}
		if (m_dwBeforeMouseState[iButton] == KEY_UP)
		{
			if (m_dwMouseState[iButton] == KEY_UP)
			{
				m_dwMouseState[iButton] = KEY_FREE;
			}
		}
		m_dwBeforeMouseState[iButton] = m_dwMouseState[iButton];
	}

	g_ActionInput.a_LeftClick = m_dwMouseState[0];
	g_ActionInput.a_RightClick = m_dwMouseState[2];
	//////////////////////////////////////////////////
	return 1;
}

bool MInput::Render()
{
	return 0;
}

bool MInput::Release()
{
	return 0;
}

LRESULT MInput::MsgProc(MSG msg)
{
	//////중계받은 메세지에 따라 마우스 배열에 넣어서 사용한다.
	switch (msg.message)
	{
	case WM_LBUTTONDOWN:
	{
		m_dwMouseState[0] = KEY_PUSH;
	}
	break;
	case WM_LBUTTONUP:
	{
		m_dwMouseState[0] = KEY_UP;
	}break;
	case WM_MBUTTONDOWN:
	{
		m_dwMouseState[1] = KEY_PUSH;
	}break;
	case WM_MBUTTONUP:
	{
		m_dwMouseState[1] = KEY_UP;
	}break;
	case WM_RBUTTONDOWN:
	{
		m_dwMouseState[2] = KEY_PUSH;
	}break;
	case WM_RBUTTONUP:
	{
		m_dwMouseState[2] = KEY_UP;
	}break;
	}
	return 1;
}

DWORD MInput::KeyCheck(DWORD dwkey)
{
	SHORT sKey = GetAsyncKeyState(dwkey);
	if (sKey & 0x8000)
	{
		if (m_dwKeyState[dwkey] == KEY_FREE ||
			m_dwKeyState[dwkey] == KEY_UP)
		{
			m_dwKeyState[dwkey] = KEY_PUSH;
		}
		else
		{
			m_dwKeyState[dwkey] = KEY_HOLD;
		}
	}
	else
	{
		if (m_dwKeyState[dwkey] == KEY_PUSH ||
			m_dwKeyState[dwkey] == KEY_HOLD)
		{
			m_dwKeyState[dwkey] = KEY_UP;
		}
		else
		{
			m_dwKeyState[dwkey] = KEY_FREE;
		}
	}
	return m_dwKeyState[dwkey];
}



//#include "MInput.h"
//
//Keyinput	g_ActionInput;
//MPoint		g_MousePos;
//
//MInput::MInput()
//{
//	m_pDirectInput = NULL;
//}
//
//
//MInput::~MInput()
//{
//}
//
//bool MInput::Init()
//{
//	HRESULT CreateDirectInputResult; // 공용 결과 변수 //
//
//	////////////////// 장치 디바이스 생성 ////////////////////
//	//// 윈도우인스턴스, 버전, 식별값, 반환받을생성주소, NULL ////
//	CreateDirectInputResult = DirectInput8Create(g_hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pDirectInput, NULL);
//	if (FAILED(CreateDirectInputResult)) return false;
//	//// 타입(키보드,마우스), 반환받을생성주소, NULL ////
//	CreateDirectInputResult = m_pDirectInput->CreateDevice(GUID_SysKeyboard, &m_pDidevKeyBoard, NULL);
//	if (FAILED(CreateDirectInputResult)) return false;
//	CreateDirectInputResult = m_pDirectInput->CreateDevice(GUID_SysMouse, &m_pDidevMouse, NULL);
//	if (FAILED(CreateDirectInputResult)) return false;
//	//////////////////////////////////////////////////////////
//
//
//	////////////////// 장치 디바이스 설정 ////////////////////
//	CreateDirectInputResult = m_pDidevKeyBoard->SetDataFormat(&c_dfDIKeyboard);
//	if (FAILED(CreateDirectInputResult)) return false;
//	CreateDirectInputResult = m_pDidevMouse->SetDataFormat(&c_dfDIMouse);
//	if (FAILED(CreateDirectInputResult)) return false;
//
//	//// DISCL_BACKGROUND		할당된 윈도우가 아닌 다른 곳에서도 장치를 언제든지 취득할 수 있어 응용프로그램이 활성화 되지 않아도 입력장치 사용 가능.
//	//// DISCL_FOREGROUND		윈도우 백그라운드로 이동시 자동 릴리즈 응용프로그램 활성화 상태에서만 입력장치 사용
//	//// DISCL_EXCLUSIVE		배타적 액세스 권한 장치 취득된 동안, 다른 인스턴스는 같은 장치의 권한을 취득 할 수 없다.
//	//// DISCL_NONEXCLUSIVE		비 배타적 액세스 권한 요구. 같은 장치에 액세스 하고 있는 다른 응용프로그램을 방해하지 않는다.
//	//// DISCL_NOWINKEY			window키 무효화
//	CreateDirectInputResult = m_pDidevKeyBoard->SetCooperativeLevel(g_hWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND | DISCL_NOWINKEY);
//	if (FAILED(CreateDirectInputResult))
//	{
//		//// 실패할 경우 재시도 ////
//		while (m_pDidevKeyBoard->Acquire() == DIERR_INPUTLOST);
//		CreateDirectInputResult = m_pDidevKeyBoard->SetCooperativeLevel(g_hWnd, DISCL_EXCLUSIVE | DISCL_BACKGROUND | DISCL_NOWINKEY);
//		if (FAILED(CreateDirectInputResult))
//		{
//			return false;
//		}
//	}
//
//	CreateDirectInputResult = m_pDidevMouse->SetCooperativeLevel(g_hWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND );
//	if (FAILED(CreateDirectInputResult))
//	{
//		while (m_pDidevMouse->Acquire() == DIERR_INPUTLOST);
//		CreateDirectInputResult = m_pDidevMouse->SetCooperativeLevel(g_hWnd, DISCL_EXCLUSIVE | DISCL_BACKGROUND );
//		if (FAILED(CreateDirectInputResult))
//		{
//			return false;
//		}
//	}
//
//	//// 버퍼 데이터 생성 ////
//	DIPROPDWORD dipdw;
//	dipdw.diph.dwSize = sizeof(DIPROPDWORD);
//	dipdw.diph.dwHeaderSize = sizeof(DIPROPHEADER);
//	dipdw.diph.dwObj = 0;
//	dipdw.diph.dwHow = DIPH_DEVICE;
//	dipdw.dwData = 256;
//	//// 장치의 동작을 정의하는 프로퍼티 설정 ////
//	CreateDirectInputResult = m_pDidevKeyBoard->SetProperty(DIPROP_BUFFERSIZE, &dipdw.diph);
//	if (FAILED(CreateDirectInputResult)) return false;
//
//	return true;
//}
//
//bool MInput::Frame()
//{
//	///////////////////마우스 무브////////////////////
//	GetCursorPos(&m_Mousrpos); // 마우스 포지션을 얻고
//	ScreenToClient(g_hWnd, &m_Mousrpos); // 스크린전체가 아닌 화면스크린으로 한정한다.
//
//	//////////////////////////////////////////////////
//	HRESULT KeyCheckResult;
//	if (m_dwImmediate)
//	{
//		ZeroMemory(m_KeyState, sizeof(BYTE)*256);
//		KeyCheckResult = m_pDidevKeyBoard->GetDeviceState(256, m_KeyState);
//		if (FAILED(KeyCheckResult))
//		{
//			while (m_pDidevKeyBoard->Acquire() == DIERR_INPUTLOST)
//			{
//				m_pDidevKeyBoard->Acquire();
//			}
//			return true;
//		}
//	}
//	else
//	{
//		if (!m_pDidevKeyBoard)     return false;
//		memset(&m_didod, 0, sizeof(DIDEVICEOBJECTDATA)* 256);
//		m_dwElements = MAX_PATH;
//		KeyCheckResult = m_pDidevKeyBoard->GetDeviceData(sizeof(DIDEVICEOBJECTDATA), m_didod, &m_dwElements, 0);
//
//		if (FAILED(KeyCheckResult))
//		{
//			m_dwElements = 0;
//			KeyCheckResult = m_pDidevKeyBoard->Acquire();
//			while (KeyCheckResult == DIERR_INPUTLOST)
//			{
//				KeyCheckResult = m_pDidevKeyBoard->Acquire();
//			}
//			return true;
//		}
//	}
//	return true;
//
//	////////////////////키보드체크////////////////////
//
//	g_ActionInput.Q = KeyCheck('Q');
//	g_ActionInput.W = KeyCheck('W');
//	g_ActionInput.E = KeyCheck('E');
//	g_ActionInput.R = KeyCheck('R');
//	g_ActionInput.T = KeyCheck('T');
//	g_ActionInput.Y = KeyCheck('Y');
//
//	g_ActionInput.A = KeyCheck('A');
//	g_ActionInput.S = KeyCheck('S');
//	g_ActionInput.D = KeyCheck('D');
//	g_ActionInput.F = KeyCheck('F');
//	g_ActionInput.G = KeyCheck('G');
//	g_ActionInput.H = KeyCheck('H');
//
//	g_ActionInput.Z = KeyCheck('Z');
//	g_ActionInput.X = KeyCheck('X');
//	g_ActionInput.C = KeyCheck('C');
//	g_ActionInput.V = KeyCheck('V');
//	g_ActionInput.B = KeyCheck('B');
//	g_ActionInput.N = KeyCheck('N');
//
//	g_ActionInput.UP = KeyCheck(VK_UP);
//	g_ActionInput.DOWN = KeyCheck(VK_DOWN);
//	g_ActionInput.LEFT = KeyCheck(VK_LEFT);
//	g_ActionInput.RIGHT = KeyCheck(VK_RIGHT);
//	g_ActionInput.SPACE = KeyCheck(VK_SPACE);
//	g_ActionInput.ENTER = KeyCheck(VK_RETURN);
//
//
//	///////////////////마우스 체크////////////////////
//	g_MousePos.x = (m_Mousrpos.x * 1) - g_rtWindowClient.right / 2; // 글로벌 변수에 좌표값을 넣어준다.
//	g_MousePos.y = (m_Mousrpos.y * -1) + g_rtWindowClient.bottom / 2;
//
//	for (int iButton = 0; iButton < 3; iButton++)
//	{
//		if (m_dwBeforeMouseState[iButton] == KEY_PUSH)
//		{
//			if (m_dwMouseState[iButton] == KEY_PUSH)
//			{
//				m_dwMouseState[iButton] = KEY_HOLD;
//			}
//		}
//		m_dwBeforeMouseState[iButton] = m_dwMouseState[iButton];
//	}
//	g_ActionInput.a_LeftClick = m_dwMouseState[0];
//	g_ActionInput.a_RightClick = m_dwMouseState[2];
//	//////////////////////////////////////////////////
//	return 1;
//}
//
//bool MInput::Render()
//{
//	return 0;
//}
//
//bool MInput::Release()
//{
//	return 0;
//}
//
//LRESULT MInput::MsgProc(MSG msg)
//{
//	//////중계받은 메세지에 따라 마우스 배열에 넣어서 사용한다.)
//	m_dwMouseState[0] = KEY_FREE;
//	m_dwMouseState[1] = KEY_FREE;
//	m_dwMouseState[2] = KEY_FREE;
//	switch (msg.message)
//	{
//	case WM_ACTIVATE:
//	{
//		bool bActive;
//		if (msg.wParam == WA_INACTIVE)
//		{
//			bActive = false;
//		}
//		else
//		{
//			bActive = true;
//		}
//		//SetAcquire(bActive);
//		break;
//	}
//
//
//	case WM_LBUTTONDOWN:
//	{
//		m_dwMouseState[0] = KEY_PUSH;
//	}break;
//	case WM_LBUTTONUP:
//	{
//		m_dwMouseState[0] = KEY_UP;
//	}break;
//	case WM_MBUTTONDOWN:
//	{
//		m_dwMouseState[1] = KEY_PUSH;
//	}break;
//	case WM_MBUTTONUP:
//	{
//		m_dwMouseState[1] = KEY_UP;
//	}break;
//	case WM_RBUTTONDOWN:
//	{
//		m_dwMouseState[2] = KEY_PUSH;
//	}break;
//	case WM_RBUTTONUP:
//	{
//		m_dwMouseState[2] = KEY_UP;
//	}break;
//	}
//	return 1;
//}
//
//DWORD MInput::KeyCheck(DWORD dwkey)
//{
//
//
//
//
//
//
//
//	SHORT sKey = GetAsyncKeyState(dwkey);
//	if (sKey & 0x8000)
//	{
//		if (m_KeyState[dwkey] == KEY_FREE || m_KeyState[dwkey] == KEY_UP)
//		{
//			m_KeyState[dwkey] = KEY_PUSH;
//		}
//		else
//		{
//			m_KeyState[dwkey] = KEY_HOLD;
//		}
//	}
//	else
//	{
//		if (m_KeyState[dwkey] == KEY_PUSH || m_KeyState[dwkey] == KEY_HOLD)
//		{
//			m_KeyState[dwkey] = KEY_UP;
//		}
//		else
//		{
//			m_KeyState[dwkey] = KEY_FREE;
//		}
//	}
//	return m_KeyState[dwkey];
//}
//
//
//
