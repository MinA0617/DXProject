#include "stdafx.h"
#include "MToolCore.h"

MToolCore::MToolCore()
{
	m_State = NOT_SET;
}


MToolCore::~MToolCore()
{
}

bool  MToolCore::Init()
{
	pushpull.Init();



	data = new MDXWirte;
	data->Init();
	I_Device.m_dxWriteList.push_back(data);
	data->AddData(L"0", D3DXVECTOR4(0, 0, 0, 1), D3DXVECTOR2(20, 10));
	data->AddData(L"0", D3DXVECTOR4(0, 0, 0, 1), D3DXVECTOR2(20, 60));

	I_CameraMgr.CreateFPSCamera_Main();
	light = &(I_LightMgr.m_List);
	light->SetLocalPosition(D3DXVECTOR3(0, 100, 0));
	light->m_ConstantLigth.m_fMultiplier = 1;
	light->m_ConstantLigth.m_fDistance = 3000;
	light->m_ConstantLigth.m_fInner = 1000;
	light->m_ConstantLigth.m_fOutner = 1500;
	light->m_ConstantLigth.m_fOffset = 100;
	light->m_ConstantLigth.m_Color = D3DXVECTOR3(1, 1, 1);
	light->m_ConstantLigth.m_iFlag = SKY;

	canvas.Init();
	return true;
}

bool  MToolCore::Frame()
{
	// 비교 //
	if (m_BeforeState != m_State)
	{
		if (m_BeforeState == PUSH)
		{
			pushpull.Update();
			pushpull.SetNonRender();
		}
	}
	m_BeforeState = m_State;
	// 스위치문 //
	switch (m_State)
	{
	case TILEING:
		if (g_ActionInput.a_LeftClick >= KEY_PUSH)
		{
			tileing.SetTile();
		}
		break;
	case BRUSH:
		if (g_ActionInput.a_LeftClick >= KEY_PUSH)
		{
			if (g_ActionInput.SHIFT >= KEY_PUSH)
			{
				if (g_ActionInput.a_LeftClick != KEY_PUSH) break;
			}
			if (g_ActionInput.CTRL >= KEY_PUSH) canvas.m_bIsEraser = true;
			else canvas.m_bIsEraser = false;
			canvas.m_bIsRealDraw = true;
		}
		else
		{
			//canvas.m_bIsRealDraw = true;
			canvas.m_bIsRealDraw = false;
		}
		canvas.Frame();
		break;
	case PUSH:
		pushpull.Frame();
		if (g_ActionInput.a_LeftClick >= KEY_PUSH)
		{
			if (g_ActionInput.CTRL >= KEY_PUSH) pushpull.m_bReversal = true;
			else pushpull.m_bReversal = false;
			pushpull.PushPull();
		}
		if (g_ActionInput.a_LeftClick == KEY_UP)
		{
			pushpull.Update();
		}
		break;
	case CREATEOBJ:
		if (g_ActionInput.a_LeftClick == KEY_PUSH)
		{
			instance.Create();
		}
		break;
	case DELETEOBJ:
		if (g_ActionInput.a_LeftClick == KEY_PUSH)
		{
			instance.Delete();
		}
		break;
	default:
		break;
	}

	// --------------- 스트링 ---------------------
	M_STR str = to_wstring(I_Timer.GetFramePerSecond());
	data->UpdateData(0, str, D3DXVECTOR4(0, 0, 0, 1), D3DXVECTOR2(20, 10));
	data->UpdateData(0, str, D3DXVECTOR4(0, 0, 0, 1), D3DXVECTOR2(20, 10));
	I_CameraMgr.m_MainCamera->isRotition = false;
	if (g_ActionInput.a_RightClick >= KEY_PUSH)
	{
		I_CameraMgr.m_MainCamera->isRotition = true;
	}
	// --------------- 빛 회전 ---------------------

	// --------------- 빛 회전 ---------------------
	if(1)
	{
		D3DXVECTOR3 tt;
		y += 1 * g_fSeoundPerFrame;
		D3DXVec3Normalize(&tt, &D3DXVECTOR3(cos(y), -1, sin(y)));
		light->m_ConstantLigth.m_Direction = tt;
	}
	// --------------- 빛 회전 ---------------------


	// --------------- 단축키 ----------------------
	if (g_ActionInput.F1 == KEY_PUSH)
	{
		if(I_3DObjectMgr.m_InWorldFiled) g_bIsLOD = !g_bIsLOD;
	}
	if (g_ActionInput.F2 == KEY_PUSH)
	{
		g_isBoxRender = !g_isBoxRender;
	}
	if (g_ActionInput.F3 >= KEY_PUSH)
	{

	}
	if (g_ActionInput.F4 == KEY_PUSH)
	{
		if (I_Device.m_RasterizerStateID == MSolidFrame)
		{
			I_Device.m_RasterizerStateID = MWireFrame;
		}
		else
		{
			I_Device.m_RasterizerStateID = MSolidFrame;
		}
	}
	if (g_ActionInput.S >= KEY_PUSH)
	{
		I_CameraMgr.m_MainCamera->MoveBack(300);
	}
	if (g_ActionInput.W >= KEY_PUSH)
	{
		I_CameraMgr.m_MainCamera->MoveFront(300);
	}
	if (g_ActionInput.A >= KEY_PUSH)
	{
		I_CameraMgr.m_MainCamera->MoveLeft(300);
	}
	if (g_ActionInput.D >= KEY_PUSH)
	{
		I_CameraMgr.m_MainCamera->MoveRight(300);
	}
	// --------------- 단축키 ----------------------
	return true;
}

bool  MToolCore::Render()
{
	switch (m_State)
	{
	case TILEING:
		break;
	case BRUSH:
		break;
	case PUSH:
		break;
	case CREATEOBJ:
		break;
	default:
		break;
	}
	return true;
}

bool  MToolCore::Release()
{
	canvas.Release();
	//I_KeyAnimationMgr.Release();
	return true;
}