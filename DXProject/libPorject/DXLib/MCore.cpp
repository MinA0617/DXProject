#include "MCore.h"

bool	g_bIsLOD;
#if defined(DEBUG) || defined(_DEBUG)
bool	g_isBoxRender;
bool	g_isPointRender;
#endif
MCore::MCore()
{
}


MCore::~MCore()
{

}

bool  MCore::Init()
{
	return true;
}

bool  MCore::Frame()
{
	return true;
}

bool  MCore::Render()
{
	return true;
}

bool  MCore::Release()
{
	return true;
}

bool MCore::MCoreInit()
{
	I_Device.Init();
	I_Timer.Init();
	I_Input.Init();
	I_MSoundMgr.Init();

	return Init();
}
bool MCore::MCoreFrame()
{
	I_Device.Frame();
	I_Timer.Frame();
	I_Input.Frame();
	I_MSoundMgr.Frame();

	return Frame();
}

bool MCore::PreRender()
{
	I_Device.PreRender();
	return true;
}

bool MCore::MCoreRender()
{
	PreRender();
	I_Device.Render();
	Render();
	PostRender();
	return true;
}
bool MCore::MCoreRelease()
{
	I_Device.Release();
	I_Timer.Release();
	I_Input.Release();
	I_MSoundMgr.Release();
	return Release();

}

void MCore::MCoreMsgProc(MSG msg)
{
	I_Input.MsgProc(msg);
	switch (msg.message)
	{
	case WM_SIZE:
	{
		UINT width = LOWORD(msg.lParam);
		UINT height = HIWORD(msg.lParam);

		GetWindowRect(m_hWnd, &m_rtWindowBounds);
		GetClientRect(g_hWnd, &m_rtWindowClient);
		g_rtWindowClient = m_rtWindowClient;
		m_iWindowWidth = m_rtWindowClient.right - m_rtWindowClient.left;
		m_iWindowHeight = m_rtWindowClient.bottom - m_rtWindowClient.top;

		ResizeDevice(width, height);
	}
	default:
		break;
	}
}

bool MCore::ResizeDevice(UINT width, UINT height)
{
	I_Device.ResizeSwapChain(width, height);
	I_RenderTargetMgr.Resize();
	if (I_CameraMgr.m_MainCamera) I_CameraMgr.m_MainCamera->Resize();
	I_Device.ResizeDXWirte();
	return true;
}

bool MCore::DebugRender()
{
	I_Timer.Render();
	I_Input.Render();
	I_MSoundMgr.Render();
	return true;
}

bool MCore::PostRender()
{
	if (m_bDebug == true)
	{
		DebugRender();
	}
	I_Device.PostRender();
	return true;
}

bool MCore::Run()
{
	MCoreInit();
	MSG msg = { 0, };
	while (WM_QUIT != msg.message)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessageW(&msg);
			MCoreMsgProc(msg);
		}
		else
		{
			MCoreFrame();
			MCoreRender();
		}
	}
	MCoreRelease();
	return true;
}
