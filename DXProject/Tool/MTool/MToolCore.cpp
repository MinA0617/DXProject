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
	inmo = nullptr;

	data = new MDXWirte;
	data->Init();
	I_Device.m_dxWriteList.push_back(data);
	data->AddData(L"0", D3DXVECTOR4(0, 0, 0, 1), D3DXVECTOR2(20, 10));
	data->AddData(L"0", D3DXVECTOR4(0, 0, 0, 1), D3DXVECTOR2(20, 60));

	filed == nullptr;
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
	// --test
	if (inmo != nullptr)
	{
		M3DInstanceModel* temp = (*I_3DObjectMgr.InstanceModelList.find(0)).second;
		M_STR str = to_wstring(temp->m_MatrixList.size());
		data->UpdateData(1, str, D3DXVECTOR4(0, 0, 0, 1), D3DXVECTOR2(20, 60));
	}
	if (g_ActionInput.Delete == KEY_PUSH)
	{
		if (inmo == nullptr)
		{
			inmo = new M3DInstanceModel;
			inmo->Init();
			I_Parser.Load(L"../../data/obj/Box001.OBJ");
			M_STR name[1];
			name[0] = L"Box001";
			int result = I_3DObjectMgr.AddInWorld(name);
			inmo->m_Mesh = I_3DObjectMgr.findObject(result)->m_pObj;
			inmo->m_Box.Copy(I_3DObjectMgr.findObject(result)->m_Box);
			I_3DObjectMgr.DeleteInWorld(result);
			inmo->m_Material->m_PixelShaderID = PS3DINSTANCE;
			inmo->m_Mesh->m_VertexShaderID = VS3DINSTANCE;

			int id = I_3DObjectMgr.AddInstanceModel(inmo);
			I_3DObjectMgr.AddInstanceObj(id, 100);
			for (int i = 0; i < 100; i++)
			{
				M3DInstance* temp = I_3DObjectMgr.GetInstanceObj(id, i);
				int x, y, z;
				x = (i * (5141 * (i + 5))) % 360;
				y = (i * (5554 * (i + 7))) % 100;
				z = (i * (4587 * (i + 8))) % 360;
				temp->SetPosition(D3DXVECTOR3(x, y, z));
				temp->SetScale(D3DXVECTOR3(1 + (i*0.02), 1 + (i*0.02), 1 + (i*0.02)));
			}
		}
	}
	// -----
	M_STR str = to_wstring(I_Timer.GetFramePerSecond());
	data->UpdateData(0, str, D3DXVECTOR4(0, 0, 0, 1), D3DXVECTOR2(20, 10));

	I_CameraMgr.m_MainCamera->isRotition = false;
	if (g_ActionInput.a_RightClick >= KEY_PUSH)
	{
		I_CameraMgr.m_MainCamera->isRotition = true;
	}

	if (g_ActionInput.a_LeftClick >= KEY_PUSH)
	{
		switch (m_State)
		{
		case TILEING:
			tileing.SetTile();
			break;
		case BRUSH:
			if (g_ActionInput.SHIFT >= KEY_PUSH)
			{
				if (g_ActionInput.a_LeftClick != KEY_PUSH) break;
			}
			if (g_ActionInput.CTRL >= KEY_PUSH) canvas.m_bIsEraser = true;
			else canvas.m_bIsEraser = false;
			canvas.Brushing();
			break;
		case PUSH:
			if (g_ActionInput.CTRL >= KEY_PUSH) pushpull.m_bReversal = true;
			else pushpull.m_bReversal = false;
			pushpull.PushPull();
			break;
		case CREATEOBJ:
			if (g_ActionInput.a_LeftClick == KEY_PUSH)
			{
				instance.Create();
			}
		default:
			break;
		}
	}

	if(1)
	{
		D3DXVECTOR3 tt;
		y += 1 * g_fSeoundPerFrame;
		D3DXVec3Normalize(&tt, &D3DXVECTOR3(cos(y), -1, sin(y)));
		light->m_ConstantLigth.m_Direction = tt;
	}

	if (g_ActionInput.F1 >= KEY_PUSH)
	{
		if(I_3DObjectMgr.m_InWorldFiled) g_bIsLOD = true;
	}
	if (g_ActionInput.F2 >= KEY_PUSH)
	{
		if (I_3DObjectMgr.m_InWorldFiled) g_bIsLOD = false;
	}
	if (g_ActionInput.F3 >= KEY_PUSH)
	{
		I_Device.m_RasterizerStateID = MSolidFrame;
	}
	if (g_ActionInput.F4 >= KEY_PUSH)
	{
		I_Device.m_RasterizerStateID = MWireFrame;
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
	return true;
}

bool  MToolCore::Render()
{
	return true;
}

bool  MToolCore::Release()
{
	canvas.Release();
	//I_KeyAnimationMgr.Release();
	return true;
}