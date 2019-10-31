#include "M3DObjectMgr.h"

ID3D11Buffer*		g_pVB_BoundingBox;
ID3D11Buffer*		g_pIB_BoundingBox;
DWORD				g_dwIC_BoundingBox;
ID3D11Buffer*		g_pVB_Point;
ID3D11Buffer*		g_pIB_Point;
DWORD				g_dwIC_Point;


M3DObject * M3DObjectMgr::find(M_STR name)
{
	ITOR data = m_List.find(name);
	if (data == m_List.end())
	{
		return nullptr;
	}
	return (*data).second;
}

MUnit* M3DObjectMgr::findUnit(M_STR name)
{
	ITORU data = m_InWorldUnitList.find(name);
	if (data == m_InWorldUnitList.end())
	{
		return nullptr;
	}
	return (*data).second;
}

MFiled * M3DObjectMgr::findFiled(M_STR name)
{
	ITORF data = m_InWorldFiledList.find(name);
	if (data == m_InWorldFiledList.end())
	{
		return nullptr;
	}
	return (*data).second;
}

M3DModel* M3DObjectMgr::findObject(int index)
{
	ITORO temp = m_InWorldObjectList.find(index);
	if (temp == m_InWorldObjectList.end())return nullptr;
	return (*temp).second;
}

M3DObject * M3DObjectMgr::operator[](M_STR name)
{
	ITOR temp = m_List.find(name);
	if (temp != m_List.end())
	{
		return (*temp).second;
	}
	return nullptr;
}

bool M3DObjectMgr::Add(M3DObject * data)
{
	ITOR temp = m_List.find(data->m_name);
	if (temp != m_List.end())
	{
		return false;
	}
	m_List.insert(make_pair(data->m_name, data));
	return true;
}

bool M3DObjectMgr::Delete(M_STR name)
{
	ITOR temp = m_List.find(name);
	if (temp == m_List.end())
	{
		return false;
	}
	delete (*temp).second;
	m_List.erase(temp);
	return true;
}

MUnit* M3DObjectMgr::CreateUnit(M_STR name, M_STR sktname, M_STR* namelist, int namecount)
{
	ITORU temp = m_InWorldUnitList.find(name);
	if (temp == m_InWorldUnitList.end())
	{
		MUnit* Unit = new MUnit;
		Unit->Create(name, sktname, namelist, namecount);
		m_InWorldUnitList.insert(make_pair(name, Unit));
		return Unit;
	}
	return nullptr;
}

MFiled * M3DObjectMgr::CreateFiled(M_STR name)
{
	ITORF temp = m_InWorldFiledList.find(name);
	if (temp == m_InWorldFiledList.end())
	{

		MFiled* filed = new MFiled;
		filed->Set(name);
		m_InWorldFiledList.insert(make_pair(name, filed));
		return filed;
	}
	return nullptr;
}

int M3DObjectMgr::AddInWorld(M_STR* namelist, int namecount)
{
	int result = -1;
	for (int i = 0; i < namecount; i++)
	{
		if (i == 0)
		{
			result = m_iObjIndex;
		}
		M3DModel* model = new M3DModel;
		model->Copy((*m_List.find(namelist[i])).second);
		m_InWorldObjectList.insert(make_pair(m_iObjIndex++, model));
	}
	return result;
}

bool M3DObjectMgr::CreateBasicBuffer()
{
	SPLINE_VERTEX ver[8];
	ZeroMemory(&ver, sizeof(SPLINE_VERTEX) * 8);
	ver[0].p = float3(1, 1, -1);
	ver[1].p = float3(-1, 1, -1);
	ver[2].p = float3(-1, -1, -1);
	ver[3].p = float3(1, -1, -1);
	ver[4].p = float3(1, 1, 1);
	ver[5].p = float3(-1, 1, 1);
	ver[6].p = float3(-1, -1, 1);
	ver[7].p = float3(1, -1, 1);

	DWORD bindex[24] = { 0,1,1,2,2,3,3,0,4,5,5,6,6,7,7,4,0,4,1,5,2,6,3,7 };
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	D3D11_BUFFER_DESC BufferDesc;				//// 공용 버퍼 데스크
	D3D11_SUBRESOURCE_DATA SubresourceData;		//// 공용 리소스 데이터
	HRESULT CreateBufferResult;					//// 공용 결과값
	///// 버텍스 버퍼 ///////////////////////////////////////////////////////////////////////////////////////////////////
	ZeroMemory(&BufferDesc, sizeof(D3D11_BUFFER_DESC));
	BufferDesc.ByteWidth = sizeof(SPLINE_VERTEX) * 8;		//// 사이즈 (Byte단위)
	BufferDesc.Usage = D3D11_USAGE_DEFAULT;							//// 사용모드 (DEFAULT는 그래픽카드에 생성)
	BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;				//// 어떤 버퍼로 사용되는지
	ZeroMemory(&SubresourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	SubresourceData.pSysMem = ver;
	if (FAILED(CreateBufferResult = g_pDevice->CreateBuffer(&BufferDesc, &SubresourceData, &g_pVB_BoundingBox))) return false;
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//// 인덱스 버퍼 /////////////////////////////////////////////////////////////////////////////////////////////////////
	ZeroMemory(&BufferDesc, sizeof(D3D11_BUFFER_DESC));
	BufferDesc.ByteWidth = sizeof(DWORD) * 24;			//// 사이즈 (Byte단위)
	BufferDesc.Usage = D3D11_USAGE_DEFAULT;							//// 사용모드 (DEFAULT는 그래픽카드에 생성)
	BufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;					//// 어떤 버퍼로 사용되는지
	ZeroMemory(&SubresourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	SubresourceData.pSysMem = bindex;
	if (FAILED(CreateBufferResult = g_pDevice->CreateBuffer(&BufferDesc, &SubresourceData, &g_pIB_BoundingBox))) return false;
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	g_dwIC_BoundingBox = 24;

	SPLINE_VERTEX data[6];
	ZeroMemory(&data, sizeof(SPLINE_VERTEX) * 6);
	data[0].p = float3(1, 0, 0);
	data[1].p = float3(-1, 0, 0);
	data[2].p = float3(0, 1, 0);
	data[3].p = float3(0, -1, 0);
	data[4].p = float3(0, 0, 1);
	data[5].p = float3(0, 0, -1);
	DWORD sindex[6];
	for (int i = 0; i < 6; i++)
	{
		sindex[i] = i;
	}

	ZeroMemory(&BufferDesc, sizeof(D3D11_BUFFER_DESC));
	BufferDesc.ByteWidth = sizeof(SPLINE_VERTEX) * 6;		//// 사이즈 (Byte단위)
	BufferDesc.Usage = D3D11_USAGE_DEFAULT;							//// 사용모드 (DEFAULT는 그래픽카드에 생성)
	BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;				//// 어떤 버퍼로 사용되는지
	ZeroMemory(&SubresourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	SubresourceData.pSysMem = &data;
	if (FAILED(CreateBufferResult = g_pDevice->CreateBuffer(&BufferDesc, &SubresourceData, &g_pVB_Point))) return false;
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//// 인덱스 버퍼 /////////////////////////////////////////////////////////////////////////////////////////////////////
	ZeroMemory(&BufferDesc, sizeof(D3D11_BUFFER_DESC));
	BufferDesc.ByteWidth = sizeof(DWORD) * 6;			//// 사이즈 (Byte단위)
	BufferDesc.Usage = D3D11_USAGE_DEFAULT;							//// 사용모드 (DEFAULT는 그래픽카드에 생성)
	BufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;					//// 어떤 버퍼로 사용되는지
	ZeroMemory(&SubresourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	SubresourceData.pSysMem = &sindex;
	if (FAILED(CreateBufferResult = g_pDevice->CreateBuffer(&BufferDesc, &SubresourceData, &g_pIB_Point))) return false;
	g_dwIC_Point = 6;

	return true;
}

bool M3DObjectMgr::Init()
{
	//m_pWorld = new M3DObject;
	I_CameraMgr.Init();
	I_LightMgr.Init();
	CreateBasicBuffer();
	return true;
}

bool M3DObjectMgr::Frame()
{

	I_CameraMgr.Frame();
	I_LightMgr.Frame();
	for (auto data : m_InWorldFiledList)
	{
		data.second->Frame();
	}
	for (auto data : m_InWorldObjectList)
	{
		data.second->Frame();
	}
	for (auto data : m_InWorldUnitList)
	{
		data.second->Frame();
	}
	return true;
}

bool M3DObjectMgr::Render()
{
	I_CameraMgr.Render();
	I_LightMgr.Render();

	//for (int i = 1; i < 5; i++)
	//{
	//	M3DModel model;
	//	model.Copy(find(L"Box001"));
	//	model.SetLocalPosition(I_CameraMgr.frustum.m_vFrustum[i]);
	//	//model->SetLocalPosition(D3DXVECTOR3(i * 100, i * 100, i * 100));
	//	model.Frame();
	//	model.Render();
	//	model.Release();
	//}
	for (auto data : m_InWorldFiledList)
	{
		for (auto box : data.second->m_BoxList)
		{
			if (I_CameraMgr.frustum.CheckOBB(box))
			{
				data.second->Render();
				break;
			}
		}
	}
	for (auto data : m_InWorldObjectList)
	{
		for (auto box : data.second->m_BoxList)
		{
			if (I_CameraMgr.frustum.CheckOBB(box))
			{
				data.second->Render();
				break;
			}
		}
	}
	for (auto data : m_InWorldUnitList)
	{
		for (auto box : data.second->m_BoxList)
		{
			if (I_CameraMgr.frustum.CheckOBB(box))
			{
				data.second->Render();
				break;
			}
		}
	}
	return true;
}

bool M3DObjectMgr::Release()
{
	SAFE_RELEASE(g_pVB_BoundingBox);
	SAFE_RELEASE(g_pIB_BoundingBox);
	SAFE_RELEASE(g_pVB_Point);
	SAFE_RELEASE(g_pIB_Point);
	//m_pWorld->Release();
	I_CameraMgr.Release();
	I_LightMgr.Release();
	I_SkeletonMgr.Release();
	for (ITOR temp = m_List.begin(); temp != m_List.end(); temp++)
	{
		(*temp).second->Release();
		delete (*temp).second;
	}
	for (auto data : m_InWorldFiledList)
	{
		data.second->Release();
		delete data.second;
	}
	for (auto data : m_InWorldUnitList)
	{
		data.second->Release();
		delete data.second;
	}
	for (auto data : m_InWorldObjectList)
	{
		data.second->Release();
		delete data.second;
	}
	m_List.clear();
	m_InWorldUnitList.clear();
	return true;
}

M3DObjectMgr::M3DObjectMgr()
{
	m_iObjIndex = 0;
}


M3DObjectMgr::~M3DObjectMgr()
{
}
