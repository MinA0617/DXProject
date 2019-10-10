#include "MSample.h"

MSample::MSample()
{
}


MSample::~MSample()
{
}

bool  MSample::Init()
{
	I_CameraMgr.CreateFPSCamera_Main();

	m_dxWrite.AddData(L"x", D2D1::ColorF(1, 1, 1, 1.0), MPoint(0, 0));
	m_dxWrite.AddData(L"y", D2D1::ColorF(1, 1, 1, 1.0), MPoint(80, 0));
	m_dxWrite.AddData(L"z", D2D1::ColorF(1, 1, 1, 1.0), MPoint(160, 0));
	m_dxWrite.AddData(L"z", D2D1::ColorF(1, 1, 1, 1.0), MPoint(240, 0));
	m_dxWrite.AddData(L"z", D2D1::ColorF(1, 1, 1, 1.0), MPoint(320, 0));
	m_dxWrite.AddData(L"z", D2D1::ColorF(1, 1, 1, 1.0), MPoint(400, 0));
	m_dxWrite.AddData(L"z", D2D1::ColorF(1, 1, 1, 1.0), MPoint(480, 0));
	m_dxWrite.AddData(L"z", D2D1::ColorF(1, 1, 1, 1.0), MPoint(560, 0));
	m_dxWrite.AddData(L"z", D2D1::ColorF(1, 1, 1, 1.0), MPoint(640, 0));

	//ps.Load(L"../../data/obj/CatGirl2.MIN");
	//ps.Load(L"../../data/obj/0.MIN");
	ps.Load(L"../../data/obj/4.MIN");

	//skt = &(*I_KeyAnimationMgr.m_KeyAniList.find(L"0.MIN")).second->m_TempSkeleton;
	// I_3DObjectMgr.Add(L"a", skt);
	//target = I_3DObjectMgr[L"catgirl_Body"];
	I_SkeletonMgr[0]->m_fAniSpeed = 1;
	light = &(I_LightMgr.m_List);

	y = 0;
	return true;
}

bool  MSample::Frame()
{
	D3DXVECTOR3 tt;
	y += 0.001;
	D3DXVec3Normalize(&tt, &D3DXVECTOR3(cos(y), -1, sin(y)));
	light->m_ConstantLigth.m_Direction = tt;

	light->m_ConstantLigth.m_fMultiplier = 1;
	light->m_ConstantLigth.m_fDistance = 3000;
	light->m_ConstantLigth.m_fInner = 1000;
	light->m_ConstantLigth.m_fOutner = 1500;
	light->m_ConstantLigth.m_fOffset = 100;
	light->m_ConstantLigth.m_Color = D3DXVECTOR3(1, 1, 1);
	light->m_ConstantLigth.m_iFlag = OMNI;
	light->Frame();
	//// 회전은 행렬에서 가져오기
	//// 스케일, 포지션은 실수값
	//wstringstream temp;
	//wstring data;
	//temp << L"FPS" << I_Timer.GetFramePerSecond() << L" posX" << target->GetPosition().x << L" posY" << target->GetPosition().y << L" posZ" << target->GetPosition().z;
	//temp >> data;
	//m_dxWrite.UpdateData(0, data, D2D1::ColorF(0, 0, 0, 1.0), MPoint(0, 0));
	//temp >> data;
	//m_dxWrite.UpdateData(1, data, D2D1::ColorF(0, 0, 0, 1.0), MPoint(80, 0));
	//temp >> data;
	//m_dxWrite.UpdateData(2, data, D2D1::ColorF(0, 0, 0, 1.0), MPoint(160, 0));
	//temp >> data;
	//m_dxWrite.UpdateData(3, data, D2D1::ColorF(0, 0, 0, 1.0), MPoint(240, 0));

	if (g_ActionInput.F1 >= KEY_PUSH)
	{
		//target = (*skt->m_BoneList.find(L"Box001")).second;
		I_3DObjectMgr.Frame();
		I_SkeletonMgr.Frame();
		//target->Frame();

	}
	if (g_ActionInput.F2 >= KEY_PUSH)
	{
		//target = (*skt->m_BoneList.find(L"Box002")).second;

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

	}
	if (g_ActionInput.F7 == KEY_PUSH)
	{
		target->UnLinkParents();

	}
	if (g_ActionInput.F8 == KEY_PUSH)
	{

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
	//if (g_ActionInput.UP >= KEY_PUSH)
	//{
	//	target->ModifyLocalPosition(D3DXVECTOR3(0, 0, 0.1));
	//	//D3DXVECTOR3 temp;
	//	//D3DXVec3Normalize(&temp, &D3DXVECTOR3(0, -1, 1));
	//	//light->m_ConstantLigth.m_Direction = temp;
	//}
	//if (g_ActionInput.DOWN>= KEY_PUSH)
	//{
	//	target->ModifyLocalPosition(D3DXVECTOR3(0, 0, -0.1));
	//	//D3DXVECTOR3 temp;
	//	//D3DXVec3Normalize(&temp, &D3DXVECTOR3(0, -1, -1));
	//	//light->m_ConstantLigth.m_Direction = temp;
	//}
	//if (g_ActionInput.RIGHT >= KEY_PUSH)
	//{
	//	target->ModifyLocalPosition(D3DXVECTOR3(0.1, 0, 0));
	//	D3DXVECTOR3 ttemp;
	//	D3DXVec3Normalize(&ttemp, &D3DXVECTOR3(-1, -1, 0));
	//	light->m_ConstantLigth.m_Direction = ttemp;
	//}
	//if (g_ActionInput.LEFT >= KEY_PUSH)
	//{
	//	target->ModifyLocalPosition(D3DXVECTOR3(-0.1, 0, 0));
	//	D3DXVECTOR3 ttemp;
	//	D3DXVec3Normalize(&ttemp, &D3DXVECTOR3(1, -1, 0));
	//	light->m_ConstantLigth.m_Direction = ttemp;
	//}
	//if (g_ActionInput.Z >= KEY_PUSH)
	//{
	//	//target->ModifyLocalPosition(D3DXVECTOR3(0, 0.1, 0));
	//}
	//if (g_ActionInput.X >= KEY_PUSH)
	//{
	//	target->ModifyLocalPosition(D3DXVECTOR3(0, -0.1, 0));
	//}
	//if (g_ActionInput.Num1 >= KEY_PUSH)
	//{
	//	D3DXQUATERNION temp;
	//	D3DXQuaternionRotationYawPitchRoll(&temp, 0, 0, 0.01);
	//	target->ModifyLocalRotation(temp);
	//}
	//if (g_ActionInput.Num2 >= KEY_PUSH)
	//{
	//	D3DXQUATERNION temp;
	//	D3DXQuaternionRotationYawPitchRoll(&temp, 0, 0, -0.01);
	//	target->ModifyLocalRotation(temp);
	//}
	//if (g_ActionInput.Num3 >= KEY_PUSH)
	//{
	//	D3DXQUATERNION temp;
	//	D3DXQuaternionRotationYawPitchRoll(&temp, 0, 0.01, 0);
	//	target->ModifyLocalRotation(temp);
	//}
	//if (g_ActionInput.Num4 >= KEY_PUSH)
	//{
	//	D3DXQUATERNION temp;
	//	D3DXQuaternionRotationYawPitchRoll(&temp, 0, -0.01, 0);
	//	target->ModifyLocalRotation(temp);
	//}
	//if (g_ActionInput.Num5 >= KEY_PUSH)
	//{
	//	D3DXQUATERNION temp;
	//	D3DXQuaternionRotationYawPitchRoll(&temp, 0.01, 0, 0);
	//	target->ModifyLocalRotation(temp);
	//}
	//if (g_ActionInput.Num6 >= KEY_PUSH)
	//{
	//	D3DXQUATERNION temp;
	//	D3DXQuaternionRotationYawPitchRoll(&temp, -0.01, 0, 0);
	//	target->ModifyLocalRotation(temp);
	//}

	//if (g_ActionInput.Insert >= KEY_PUSH)
	//{
	//	target->ModifyLocalScale(D3DXVECTOR3(0.001, 0, 0));
	//}
	//if (g_ActionInput.Home >= KEY_PUSH)
	//{
	//	target->ModifyLocalScale(D3DXVECTOR3(0, 0.001, 0));
	//}
	//if (g_ActionInput.PgUp >= KEY_PUSH)
	//{
	//	target->ModifyLocalScale(D3DXVECTOR3(0, 0, 0.001));
	//}
	//if (g_ActionInput.Delete >= KEY_PUSH)
	//{
	//	target->ModifyLocalScale(D3DXVECTOR3(-0.001, 0, 0));
	//}
	//if (g_ActionInput.End >= KEY_PUSH)
	//{
	//	target->ModifyLocalScale(D3DXVECTOR3(0, -0.001, 0));
	//}
	//if (g_ActionInput.PgDn >= KEY_PUSH)
	//{
	//	target->ModifyLocalScale(D3DXVECTOR3(0, 0, -0.001));
	//}
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