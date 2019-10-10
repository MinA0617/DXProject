#include "MCore.h"

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
	m_dxWrite.Init();
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
	m_dxWrite.Frame();

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
	m_dxWrite.Release();
	I_MSoundMgr.Release();
	return Release();
}

void MCore::MCoreMsgProc(MSG msg)
{
	I_Input.MsgProc(msg);
}

bool MCore::ResizeDevice(UINT width, UINT height)
{
	I_Device.ResizeSwapChain(width, height);
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
	m_dxWrite.Render();
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
