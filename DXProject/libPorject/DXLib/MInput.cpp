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
	////////////////////Ű����üũ////////////////////
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


	///////////////////���콺 üũ////////////////////
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
	//////�߰���� �޼����� ���� ���콺 �迭�� �־ ����Ѵ�.
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
//	HRESULT CreateDirectInputResult; // ���� ��� ���� //
//
//	////////////////// ��ġ ����̽� ���� ////////////////////
//	//// �������ν��Ͻ�, ����, �ĺ���, ��ȯ���������ּ�, NULL ////
//	CreateDirectInputResult = DirectInput8Create(g_hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pDirectInput, NULL);
//	if (FAILED(CreateDirectInputResult)) return false;
//	//// Ÿ��(Ű����,���콺), ��ȯ���������ּ�, NULL ////
//	CreateDirectInputResult = m_pDirectInput->CreateDevice(GUID_SysKeyboard, &m_pDidevKeyBoard, NULL);
//	if (FAILED(CreateDirectInputResult)) return false;
//	CreateDirectInputResult = m_pDirectInput->CreateDevice(GUID_SysMouse, &m_pDidevMouse, NULL);
//	if (FAILED(CreateDirectInputResult)) return false;
//	//////////////////////////////////////////////////////////
//
//
//	////////////////// ��ġ ����̽� ���� ////////////////////
//	CreateDirectInputResult = m_pDidevKeyBoard->SetDataFormat(&c_dfDIKeyboard);
//	if (FAILED(CreateDirectInputResult)) return false;
//	CreateDirectInputResult = m_pDidevMouse->SetDataFormat(&c_dfDIMouse);
//	if (FAILED(CreateDirectInputResult)) return false;
//
//	//// DISCL_BACKGROUND		�Ҵ�� �����찡 �ƴ� �ٸ� �������� ��ġ�� �������� ����� �� �־� �������α׷��� Ȱ��ȭ ���� �ʾƵ� �Է���ġ ��� ����.
//	//// DISCL_FOREGROUND		������ ��׶���� �̵��� �ڵ� ������ �������α׷� Ȱ��ȭ ���¿����� �Է���ġ ���
//	//// DISCL_EXCLUSIVE		��Ÿ�� �׼��� ���� ��ġ ���� ����, �ٸ� �ν��Ͻ��� ���� ��ġ�� ������ ��� �� �� ����.
//	//// DISCL_NONEXCLUSIVE		�� ��Ÿ�� �׼��� ���� �䱸. ���� ��ġ�� �׼��� �ϰ� �ִ� �ٸ� �������α׷��� �������� �ʴ´�.
//	//// DISCL_NOWINKEY			windowŰ ��ȿȭ
//	CreateDirectInputResult = m_pDidevKeyBoard->SetCooperativeLevel(g_hWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND | DISCL_NOWINKEY);
//	if (FAILED(CreateDirectInputResult))
//	{
//		//// ������ ��� ��õ� ////
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
//	//// ���� ������ ���� ////
//	DIPROPDWORD dipdw;
//	dipdw.diph.dwSize = sizeof(DIPROPDWORD);
//	dipdw.diph.dwHeaderSize = sizeof(DIPROPHEADER);
//	dipdw.diph.dwObj = 0;
//	dipdw.diph.dwHow = DIPH_DEVICE;
//	dipdw.dwData = 256;
//	//// ��ġ�� ������ �����ϴ� ������Ƽ ���� ////
//	CreateDirectInputResult = m_pDidevKeyBoard->SetProperty(DIPROP_BUFFERSIZE, &dipdw.diph);
//	if (FAILED(CreateDirectInputResult)) return false;
//
//	return true;
//}
//
//bool MInput::Frame()
//{
//	///////////////////���콺 ����////////////////////
//	GetCursorPos(&m_Mousrpos); // ���콺 �������� ���
//	ScreenToClient(g_hWnd, &m_Mousrpos); // ��ũ����ü�� �ƴ� ȭ�齺ũ������ �����Ѵ�.
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
//	////////////////////Ű����üũ////////////////////
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
//	///////////////////���콺 üũ////////////////////
//	g_MousePos.x = (m_Mousrpos.x * 1) - g_rtWindowClient.right / 2; // �۷ι� ������ ��ǥ���� �־��ش�.
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
//	//////�߰���� �޼����� ���� ���콺 �迭�� �־ ����Ѵ�.)
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
