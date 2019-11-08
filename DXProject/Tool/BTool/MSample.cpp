#include "stdafx.h"
#include "MSample.h"


MSample::MSample()
{
}


MSample::~MSample()
{
}


bool  MSample::Init()
{

	M_STR asd;
	M_STR asdsda = asd;
	I_CameraMgr.CreateFPSCamera_Main();
	//I_Device.m_dxWrite.AddData(L"카메라이동 : WASD", D2D1::ColorF(0, 0, 0, 1.0), MPoint(0, 0));
	//I_Device.m_dxWrite.AddData(L"오브젝트 선택 : F1, F2", D2D1::ColorF(0, 0, 0, 1.0), MPoint(30, 0));
	//I_Device.m_dxWrite.AddData(L"오브젝트 이동 : 상화좌우zx", D2D1::ColorF(0, 0, 0, 1.0), MPoint(60, 0));
	//I_Device.m_dxWrite.AddData(L"오브젝트 회전 : 숫자패드123456", D2D1::ColorF(0, 0, 0, 1.0), MPoint(90, 0));
	//I_Device.m_dxWrite.AddData(L"오브젝트 스케일 : 인서트딜리트홈엔드페이지업다운키", D2D1::ColorF(0, 0, 0, 1.0), MPoint(120, 0));

	if (1)
	{
		//ps.Load_HM(L"../../data/image/jja.bmp", 100);
		filed = ps.Load_HM(L"../../data/image/Map513.bmp", 10, true, 5);
		//filed = I_3DObjectMgr.findFiled(L"Map33");
		filed->ground->SetLevelOfDetail(250);
		filed->ground->SetColor(D3DXVECTOR3(0.5, 0.5, 0.5));
		filed->ground->Load_MAP(L"../../data/image/Map513_Minimap.png", DIFFUSE);
		//filed->ground->m_bIsLOD = true;
	}
	if (0)
	{
		ps.Load(L"../../data/obj/Catgirl_T.MIN");
		ps.Load(L"../../data/obj/Catgirl_Wait.MIN");
		ps.Load(L"../../data/obj/Catgirl_Walk.MIN");
		ps.Load(L"../../data/obj/Catgirl_Run.MIN");
		ps.Load(L"../../data/obj/Catgirl_Attack.MIN");
		ps.Load(L"../../data/obj/Catgirl_Armor.MIN", true, false, I_SkeletonMgr[L"Catgirl_T"]);
		M_STR namelist[7];
		namelist[0] = L"Catgirl_Eyes";
		namelist[1] = L"Catgirl_Head";
		namelist[2] = L"Catgirl_Hair";
		namelist[3] = L"Catgirl_UpperBody"; // L"Armor1";
		namelist[4] = L"Catgirl_LowerBody";
		namelist[5] = L"Catgirl_Hands";
		namelist[6] = L"Catgirl_Feet";
		// EYES, HEAD, HAIR, UPPERBODY, LOWERBODY, HANDS, FEET
		MUnit* temp = I_3DObjectMgr.CreateUnit(L"aa", L"Catgirl_T", namelist, _countof(namelist));
		ps.Load_BB(L"../../data/obj/Catgirl_BB.MIN", temp);
		//target = I_3DObjectMgr[L"catgirl_Body"];
		//I_MaterialMgr[target->MaterialID]->m_VertexShaderID = VS3D;
		//target = I_SkeletonMgr[0];
		unit = temp;
	}
	ps.Load(L"../../data/obj/box.MIN");
	M_STR namelist[12];
	namelist[0] = L"Box001";
	namelist[1] = L"Box002";
	namelist[2] = L"Box003";
	namelist[3] = L"Box004";
	namelist[4] = L"Box005";
	namelist[5] = L"Box006";
	namelist[6] = L"Box007";
	namelist[7] = L"Box008";
	namelist[8] = L"Box009";
	namelist[9] = L"Box010";
	namelist[10] = L"Box011";
	namelist[11] = L"Box012";
	if (0)
	{
		y = I_3DObjectMgr.AddInWorld(namelist, 2);
		box1 = I_3DObjectMgr.findObject(y);
		box2 = I_3DObjectMgr.findObject(y + 1);
	}
	light = &(I_LightMgr.m_List);
	target = I_3DObjectMgr.findObject(y);
	return true;
}

bool  MSample::Frame()
{
	D3DXVECTOR3 tt;
	y += 1 * g_fSeoundPerFrame;
	D3DXVec3Normalize(&tt, &D3DXVECTOR3(cos(y), -1, sin(y)));
	light->m_ConstantLigth.m_Direction = tt;

	light->m_ConstantLigth.m_fMultiplier = 1;
	light->m_ConstantLigth.m_fDistance = 3000;
	light->m_ConstantLigth.m_fInner = 1000;
	light->m_ConstantLigth.m_fOutner = 1500;
	light->m_ConstantLigth.m_fOffset = 100;
	light->m_ConstantLigth.m_Color = D3DXVECTOR3(1, 1, 1);
	light->m_ConstantLigth.m_iFlag = SKY;
	light->Frame();
	//// 회전은 행렬에서 가져오기
	//// 스케일, 포지션은 실수값
	//wstringstream temp;
	//wstring data;
	//temp << L"FPS_" << I_Timer.GetFramePerSecond();
	//temp >> data;
	//I_Device.m_dxWrite.UpdateData(0, data, D2D1::ColorF(0, 0, 0, 1.0), MPoint(0, 0));
	//temp >> data;
	//m_dxWrite.UpdateData(1, data, D2D1::ColorF(0, 0, 0, 1.0), MPoint(80, 0));
	//temp >> data;
	//m_dxWrite.UpdateData(2, data, D2D1::ColorF(0, 0, 0, 1.0), MPoint(160, 0));
	//temp >> data;
	//m_dxWrite.UpdateData(3, data, D2D1::ColorF(0, 0, 0, 1.0), MPoint(240, 0));

	//if (MCollision::OBBtoOBB(box1->m_BoxList[0], box2->m_BoxList[0]))
	//{
	//	int a1 = 0;
	//	box1->GetMatarial()->SetOpacity(150);
	//	box2->GetMatarial()->SetOpacity(150);
	//	box1->Render();
	//	//box1->ModifyLocalScale(D3DXVECTOR3(-0.001, -0.001, -0.001));
	//	//box2->ModifyLocalScale(D3DXVECTOR3(-0.001, -0.001, -0.001));
	//}
	//else
	//{
	//	box1->GetMatarial()->SetOpacity(255);
	//	box2->GetMatarial()->SetOpacity(255);
	//}

	if (g_ActionInput.F1 >= KEY_PUSH)
	{
		filed->ground->m_bIsLOD = true;
		//target = I_3DObjectMgr.findObject(0);
		//target->Change(UPPERBODY, L"Armor1");
	}
	if (g_ActionInput.F2 >= KEY_PUSH)
	{
		filed->ground->m_bIsLOD = false;
		//target = I_3DObjectMgr.findObject(1);
		//target->Change(UPPERBODY, L"Catgirl_UpperBody");
		//target->skt->BindAni(L"Catgirl_Wait", false, 0.15);
	}
	if (g_ActionInput.F3 >= KEY_PUSH)
	{
		I_Device.m_RasterizerStateID = MSolidFrame;
	}
	if (g_ActionInput.F4 >= KEY_PUSH)
	{
		I_Device.m_RasterizerStateID = MWireFrame;
	}
	if (g_ActionInput.F6 == KEY_PUSH)
	{
		unit->skt->BindAni(L"Catgirl_Attack", true, 0.15);
	}
	if (g_ActionInput.F7 == KEY_PUSH)
	{
		unit->skt->BindAni(L"Catgirl_Walk", true, 0.15);
	}
	if (g_ActionInput.F8 == KEY_PUSH)
	{
		unit->skt->BindAni(L"Catgirl_Run", true, 0.15);
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
	if (g_ActionInput.ENTER == KEY_PUSH)
	{
		I_CameraMgr.m_MainCamera->isRotition = !I_CameraMgr.m_MainCamera->isRotition;
	}
	if (g_ActionInput.UP >= KEY_PUSH)
	{
		target->ModifyLocalPosition(D3DXVECTOR3(0, 0, 0.1));
		//D3DXVECTOR3 temp;
		//D3DXVec3Normalize(&temp, &D3DXVECTOR3(0, -1, 1));
		//light->m_ConstantLigth.m_Direction = temp;
	}
	if (g_ActionInput.DOWN >= KEY_PUSH)
	{
		target->ModifyLocalPosition(D3DXVECTOR3(0, 0, -0.1));
		//D3DXVECTOR3 temp;
		//D3DXVec3Normalize(&temp, &D3DXVECTOR3(0, -1, -1));
		//light->m_ConstantLigth.m_Direction = temp;
	}
	if (g_ActionInput.RIGHT >= KEY_PUSH)
	{
		target->ModifyLocalPosition(D3DXVECTOR3(0.1, 0, 0));
		D3DXVECTOR3 ttemp;
		D3DXVec3Normalize(&ttemp, &D3DXVECTOR3(-1, -1, 0));
		light->m_ConstantLigth.m_Direction = ttemp;
	}
	if (g_ActionInput.LEFT >= KEY_PUSH)
	{
		target->ModifyLocalPosition(D3DXVECTOR3(-0.1, 0, 0));
		D3DXVECTOR3 ttemp;
		D3DXVec3Normalize(&ttemp, &D3DXVECTOR3(1, -1, 0));
		light->m_ConstantLigth.m_Direction = ttemp;
	}
	if (g_ActionInput.Z >= KEY_PUSH)
	{
		target->ModifyLocalPosition(D3DXVECTOR3(0, 0.1, 0));
	}
	if (g_ActionInput.X >= KEY_PUSH)
	{
		target->ModifyLocalPosition(D3DXVECTOR3(0, -0.1, 0));
	}
	if (g_ActionInput.Num1 >= KEY_PUSH)
	{
		D3DXQUATERNION temp;
		D3DXQuaternionRotationYawPitchRoll(&temp, 0, 0.001, 0);
		target->ModifyLocalRotation(temp);
	}
	if (g_ActionInput.Num2 >= KEY_PUSH)
	{
		D3DXQUATERNION temp;
		D3DXQuaternionRotationYawPitchRoll(&temp, 0, 0, 0.001);
		target->ModifyLocalRotation(temp);
	}
	if (g_ActionInput.Num3 >= KEY_PUSH)
	{
		D3DXQUATERNION temp;
		D3DXQuaternionRotationYawPitchRoll(&temp, 0.001, 0, 0);
		target->ModifyLocalRotation(temp);
	}
	if (g_ActionInput.Num4 >= KEY_PUSH)
	{
		D3DXQUATERNION temp;
		D3DXQuaternionRotationYawPitchRoll(&temp, 0, -0.001, 0);
		target->ModifyLocalRotation(temp);
	}
	if (g_ActionInput.Num5 >= KEY_PUSH)
	{
		D3DXQUATERNION temp;
		D3DXQuaternionRotationYawPitchRoll(&temp, 0, 0, -0.001);
		target->ModifyLocalRotation(temp);
	}
	if (g_ActionInput.Num6 >= KEY_PUSH)
	{
		D3DXQUATERNION temp;
		D3DXQuaternionRotationYawPitchRoll(&temp, -0.001, 0, 0);
		target->ModifyLocalRotation(temp);
	}

	if (g_ActionInput.Insert >= KEY_PUSH)
	{
		target->ModifyLocalScale(D3DXVECTOR3(0.001, 0, 0));
	}
	if (g_ActionInput.Home >= KEY_PUSH)
	{
		target->ModifyLocalScale(D3DXVECTOR3(0, 0.001, 0));
	}
	if (g_ActionInput.PgUp >= KEY_PUSH)
	{
		target->ModifyLocalScale(D3DXVECTOR3(0, 0, 0.001));
	}
	if (g_ActionInput.Delete >= KEY_PUSH)
	{
		target->ModifyLocalScale(D3DXVECTOR3(-0.001, 0, 0));
	}
	if (g_ActionInput.End >= KEY_PUSH)
	{
		target->ModifyLocalScale(D3DXVECTOR3(0, -0.001, 0));
	}
	if (g_ActionInput.PgDn >= KEY_PUSH)
	{
		target->ModifyLocalScale(D3DXVECTOR3(0, 0, -0.001));
	}
	return true;
}

bool  MSample::Render()
{
	return true;
}

bool  MSample::Release()
{
	//I_KeyAnimationMgr.Release();
	return true;
}