#include "MSample.h"

MSample::MSample()
{
}


MSample::~MSample()
{
}

bool  MSample::Init()
{
	I_3DObjectMgr.CreateFiled(L"fgh", 17, 10, 4);
	M3DHeightMap* hm = I_3DObjectMgr.m_InWorldFiled->ground;
	if (hm->CreateAlphaTexture(2048))
	{
		hm->Load_MAP(L"../../data/image/Icon_NewFile.bmp");
		hm->Load_MAP(L"../../data/image/asd.bmp");
		for (auto temp : hm->m_List)
		{
			hm->SetTexture(temp, 0, DIFFUSE, 0);
			hm->SetTexture(temp, 1, DIFFUSE, 1);
		}
	}

	I_CameraMgr.CreateFPSCamera_Main();
	I_Device.m_dxWrite.AddData(L"카메라이동 : WASD", D2D1::ColorF(0, 0, 0, 1.0), D3DXVECTOR2(0, 0));
	I_Device.m_dxWrite.AddData(L"아이템 변경 : F1, F2", D2D1::ColorF(0, 0, 0, 1.0), D3DXVECTOR2(30, 0));
	I_Device.m_dxWrite.AddData(L"애니메이션 변경 F6 ~ F9 : 상화좌우zx", D2D1::ColorF(0, 0, 0, 1.0), D3DXVECTOR2(60, 0));

	if (0)
	{
		//ps.Load_HM(L"../../data/image/jja.bmp", 100);
		filed = I_Parser.Load_HM(L"../../data/image/Map513.bmp", 10, true, 5);
		//filed = I_3DObjectMgr.findFiled(L"Map33");
		filed->ground->SetColor(D3DXVECTOR3(0.5, 0.5, 0.5));
		filed->ground->Load_MAP(L"../../data/image/Map513_Minimap.png", DIFFUSE);
		//filed->ground->m_bIsLOD = true;
	}
	if(0)
	{
		I_Parser.Load(L"../../data/obj/CatGirl.SKT");
		I_Parser.Load(L"../../data/obj/CatGirl_Attack.KEY");
		I_Parser.Load(L"../../data/obj/CatGirl_Run.KEY");
		I_Parser.Load(L"../../data/obj/CatGirl_Wait.KEY");
		I_Parser.Load(L"../../data/obj/CatGirl_Walk.KEY");
		I_Parser.Load(L"../../data/obj/CatGirl.OBJ", I_SkeletonMgr[L"CatGirl"]);
		I_Parser.Load(L"../../data/obj/CatGirl_Armor.OBJ", I_SkeletonMgr[L"CatGirl"]);
		I_Parser.Load_BB(L"../../data/obj/Catgirl_BB.OBJ", I_SkeletonMgr[L"CatGirl"]);

		M_STR namelist[7];
		namelist[0] = L"Catgirl_Eyes";
		namelist[1] = L"Catgirl_Head";
		namelist[2] = L"Catgirl_Hair";
		namelist[3] = L"Catgirl_UpperBody"; // L"Armor1";
		namelist[4] = L"Catgirl_LowerBody";
		namelist[5] = L"Catgirl_Hands";
		namelist[6] = L"Catgirl_Feet";
		// EYES, HEAD, HAIR, UPPERBODY, LOWERBODY, HANDS, FEET
		unit = I_3DObjectMgr.CreateUnit(L"aa", L"CatGirl", namelist, _countof(namelist));
 		unit->skt->BindAni(L"CatGirl_Wait");
	}


	light = &(I_LightMgr.m_List);
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
	wstringstream temp;
	wstring data;
	temp << L"FPS_" << I_Timer.GetFramePerSecond();
	temp >> data;
	I_Device.m_dxWrite.UpdateData(0, data, D2D1::ColorF(0, 0, 0, 1.0), D3DXVECTOR2(0, 0));
	temp >> data;
	//I_Device.m_dxWrite.UpdateData(1, data, D2D1::ColorF(0, 0, 0, 1.0), D3DXVECTOR2(80, 0));
	//temp >> data;
	//I_Device.m_dxWrite.UpdateData(2, data, D2D1::ColorF(0, 0, 0, 1.0), D3DXVECTOR2(160, 0));
	//temp >> data;
	//I_Device.m_dxWrite.UpdateData(3, data, D2D1::ColorF(0, 0, 0, 1.0), D3DXVECTOR2(240, 0));

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
		//filed->ground->m_bIsLOD = true;
		//target = I_3DObjectMgr.findObject(0);
		unit->Change(UPPERBODY, L"Armor1");
	}
	if (g_ActionInput.F2 >= KEY_PUSH)
	{
		//filed->ground->m_bIsLOD = false;
		target = I_3DObjectMgr.findObject(1);
		unit->Change(UPPERBODY, L"Catgirl_UpperBody");
		//target->skt->BindAni(L"Catgirl_Wait", false, 0.15);
	}
	if (g_ActionInput.F1 == KEY_PUSH)
	{
		g_isBoxRender = !g_isBoxRender;
	}
	if (g_ActionInput.F2 == KEY_PUSH)
	{
		g_isPointRender = !g_isPointRender;
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
		unit->skt->BindAni(L"CatGirl_Attack", true, 10);
	}
	if (g_ActionInput.F7 == KEY_PUSH)
	{
		unit->skt->BindAni(L"CatGirl_Walk", true, 10);
	}
	if (g_ActionInput.F8 == KEY_PUSH)
	{
		unit->skt->BindAni(L"CatGirl_Run", true, 10);
	}
	if (g_ActionInput.F9 == KEY_PUSH)
	{
		unit->skt->BindAni(L"CatGirl_Wait", false, 10);
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
	if (g_ActionInput.DOWN>= KEY_PUSH)
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

//bool Filed::Init()
//{
//	heigth.Load(L"../../data/image/Map513.bmp");
//
//	constant.g_FiledSize = 3200;
//	constant.g_TileSize = 100;
//	constant.g_Heigth = 100;
//	constant.temp = 0;
//
//	filedvecter vertices[10];
//	for (int i = 0; i < 10; i++)
//	{
//		vertices[i].Position = D3DXVECTOR2(0, 100 * i);
//		vertices[i].NeighborType = 12;
//		vertices[i].DivisionCount = 1;
//	}
//	vertices[0].Position = D3DXVECTOR2(0, 100);
//
//
//
//	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	D3D11_BUFFER_DESC BufferDesc;				//// 공용 버퍼 데스크
//	D3D11_SUBRESOURCE_DATA SubresourceData;		//// 공용 리소스 데이터
//	HRESULT CreateBufferResult;					//// 공용 결과값
//	///// 버텍스 버퍼 ///////////////////////////////////////////////////////////////////////////////////////////////////
//	ZeroMemory(&BufferDesc, sizeof(D3D11_BUFFER_DESC));
//	BufferDesc.ByteWidth = sizeof(filedvecter) * _countof(vertices);	//// 사이즈 (Byte단위)
//	BufferDesc.Usage = D3D11_USAGE_DEFAULT;							//// 사용모드 (DEFAULT는 그래픽카드에 생성)
//	BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;				//// 어떤 버퍼로 사용되는지
//	ZeroMemory(&SubresourceData, sizeof(D3D11_SUBRESOURCE_DATA));
//	SubresourceData.pSysMem = vertices;
//	if (FAILED(CreateBufferResult = g_pDevice->CreateBuffer(&BufferDesc, &SubresourceData, &vb))) return false;
//	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	//// 상수 버퍼 ///////////////////////////////////////////////////////////////////////////////////////////////////////								
//	ZeroMemory(&BufferDesc, sizeof(D3D11_BUFFER_DESC));
//	BufferDesc.ByteWidth = sizeof(CONSTANT_2DOBJ);
//	BufferDesc.Usage = D3D11_USAGE_DYNAMIC;
//	BufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
//	BufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
//	if (FAILED(CreateBufferResult = g_pDevice->CreateBuffer(&BufferDesc, NULL, &cb))) return false;
//	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	return true;
//}
//
//bool Filed::Frame()
//{
//	if (cb)
//	{
//		g_pImmediateContext->UpdateSubresource(cb, 0, 0, &constant, 0, 0);
//	}
//	return true;
//}
//
//bool Filed::Render()
//{
//	I_DxState.RS_Set(MNoneCulling);
//	I_DxState.BS_Set(MAlphaBlend);
//	I_DxState.SS_Set(MWrapLinear);
//	I_DxState.DSS_Set(MDepthEnable);
//
//	ID3D11ShaderResourceView* temp = heigth.GetTexture();
//	g_pImmediateContext->PSSetShaderResources(0, 1, &temp);
//	g_pImmediateContext->PSSetShader(I_PixelShaderMgr.m_PSList[PSFILED2], NULL, 0);
//	g_pImmediateContext->PSSetConstantBuffers(0, 1, &I_CameraMgr.m_pGrobalCameraBuffer);
//	g_pImmediateContext->PSSetConstantBuffers(1, 1, &cb);
//
//	g_pImmediateContext->GSSetShaderResources(0, 1, &temp);
//	g_pImmediateContext->GSSetShader(I_GeometryShaderMgr.m_GSList[GSFILED], NULL, 0);
//	g_pImmediateContext->PSSetConstantBuffers(1, 1, &cb);
//
//	g_pImmediateContext->IASetInputLayout(I_VertexShaderMgr.m_LOList[VSFILED2]);		// 레이아웃 셋
//	g_pImmediateContext->VSSetShader(I_VertexShaderMgr.m_VSList[VSFILED2], NULL, 0);
//	UINT stride = sizeof(filedvecter);		//// 보폭(카운트할 사이즈 크기)
//	UINT offset = 0;
//	g_pImmediateContext->IASetVertexBuffers(0, 1, &vb, &stride, &offset);
//	g_pImmediateContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_POINTLIST);
//	g_pImmediateContext->DrawAuto();
//
//	return true;
//}
//
//bool Filed::Release()
//{
//	SAFE_RELEASE(cb);
//	SAFE_RELEASE(vb);
//	heigth.Release();
//	return true;
//}
