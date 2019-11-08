#include "MParser.h"

MParser::MParser()
{
}

MParser::~MParser()
{
}

long long MParser::DataToLong(ITOR & data)
{
	string temp = *data;
	long long l = atoll(temp.c_str());
	data++;
	return l;
}

int MParser::DataToInt(ITOR &data)
{
	string temp = *data;
	int i = atoi(temp.c_str());
	data++;
	return i;
}

float MParser::DataToFloat(ITOR &data)
{
	string temp = *data;
	float f = atof(temp.c_str());
	data++;
	return f;
}

MAPTYPE MParser::DataToMAPTYPE(ITOR & data)
{
	string temp = *data;
	int type = atoi(temp.c_str());
	data++;
	switch (type)
	{
	case 1:
		return MAPTYPE(DIFFUSE);
		break;
	default:
		return MAPTYPE(NOTFOUND);
		break;
	}
}

//string MParser::DataToSTR(ITOR &data)
//{
//	string temp = *data;
//	data++;
//	return temp;
//}

M_STR MParser::DataToMSTR(ITOR & data)
{
	string temp = *data;
	data++;
	M_STR mstr;
	mstr.append(temp.begin(), temp.end());
	return mstr;
}

float3 MParser::FindMin(vector<MVERTEX>& vertices)
{
	int size = vertices.size();
	if (size == 0)
	{
		return float3(0, 0, 0);
	}
	float x, y, z;
	x = vertices[0].p.x;
	y = vertices[0].p.y;
	z = vertices[0].p.z;
	for (int i = 1; i < size; i++)
	{
		if (x > vertices[i].p.x) x = vertices[i].p.x;
		if (y > vertices[i].p.y) y = vertices[i].p.y;
		if (z > vertices[i].p.z) z = vertices[i].p.z;
	}
	return float3(x, y, z);
}

float3 MParser::FindMax(vector<MVERTEX>& vertices)
{
	int size = vertices.size();
	if (size == 0)
	{
		return float3(0, 0, 0);
	}
	float x, y, z;
	x = vertices[0].p.x;
	y = vertices[0].p.y;
	z = vertices[0].p.z;
	for (int i = 1; i < size; i++)
	{
		if (x < vertices[i].p.x) x = vertices[i].p.x;
		if (y < vertices[i].p.y) y = vertices[i].p.y;
		if (z < vertices[i].p.z) z = vertices[i].p.z;
	}
	return float3(x, y, z);
}

bool MParser::CreateBuffer(MMesh * Target, vector<MVERTEX>& vertices, vector<DWORD>& index)
{
	if (vertices.size() == 0 || index.size() == 0)
	{
		return false;
	}
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	D3D11_BUFFER_DESC BufferDesc;				//// 공용 버퍼 데스크
	D3D11_SUBRESOURCE_DATA SubresourceData;		//// 공용 리소스 데이터
	HRESULT CreateBufferResult;					//// 공용 결과값
	///// 버텍스 버퍼 ///////////////////////////////////////////////////////////////////////////////////////////////////
	ZeroMemory(&BufferDesc, sizeof(D3D11_BUFFER_DESC));
	BufferDesc.ByteWidth = sizeof(MVERTEX) * vertices.size();		//// 사이즈 (Byte단위)
	BufferDesc.Usage = D3D11_USAGE_DEFAULT;							//// 사용모드 (DEFAULT는 그래픽카드에 생성)
	BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;				//// 어떤 버퍼로 사용되는지
	ZeroMemory(&SubresourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	SubresourceData.pSysMem = &vertices.at(0);
	if (FAILED(CreateBufferResult = g_pDevice->CreateBuffer(&BufferDesc, &SubresourceData, &Target->m_pVertexBuffer))) return false;
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//// 인덱스 버퍼 /////////////////////////////////////////////////////////////////////////////////////////////////////
	ZeroMemory(&BufferDesc, sizeof(D3D11_BUFFER_DESC));
	BufferDesc.ByteWidth = sizeof(DWORD) * index.size();			//// 사이즈 (Byte단위)
	BufferDesc.Usage = D3D11_USAGE_DEFAULT;							//// 사용모드 (DEFAULT는 그래픽카드에 생성)
	BufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;					//// 어떤 버퍼로 사용되는지
	ZeroMemory(&SubresourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	SubresourceData.pSysMem = &index.at(0);
	if (FAILED(CreateBufferResult = g_pDevice->CreateBuffer(&BufferDesc, &SubresourceData, &Target->m_pIndexBuffer))) return false;
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	return true;
}

//bool MParser::CreateBuffer(MSkinModel * unit)
//{
//	vector<MVERTEX>		mv;
//	vector<SKINDATA>	ms;
//	unit->SetRCVertex(mv, ms);
//	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	D3D11_BUFFER_DESC BufferDesc;				//// 공용 버퍼 데스크
//	D3D11_SUBRESOURCE_DATA SubresourceData;		//// 공용 리소스 데이터
//	HRESULT CreateBufferResult;					//// 공용 결과값
//	///// 버텍스 버퍼 ///////////////////////////////////////////////////////////////////////////////////////////////////
//	ZeroMemory(&BufferDesc, sizeof(D3D11_BUFFER_DESC));
//	BufferDesc.ByteWidth = sizeof(MVERTEX) * mv.size();		//// 사이즈 (Byte단위)
//	BufferDesc.Usage = D3D11_USAGE_DEFAULT;							//// 사용모드 (DEFAULT는 그래픽카드에 생성)
//	BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;				//// 어떤 버퍼로 사용되는지
//	ZeroMemory(&SubresourceData, sizeof(D3D11_SUBRESOURCE_DATA));
//	SubresourceData.pSysMem = &mv.at(0);
//	if (FAILED(CreateBufferResult = g_pDevice->CreateBuffer(&BufferDesc, &SubresourceData, &I_MeshMgr[unit->ObjID]->m_pVertexBuffer))) return false;
//	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	///// 버텍스 버퍼 ///////////////////////////////////////////////////////////////////////////////////////////////////
//	ZeroMemory(&BufferDesc, sizeof(D3D11_BUFFER_DESC));
//	BufferDesc.ByteWidth = sizeof(SKINDATA) * ms.size();		//// 사이즈 (Byte단위)
//	BufferDesc.Usage = D3D11_USAGE_DEFAULT;							//// 사용모드 (DEFAULT는 그래픽카드에 생성)
//	BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;				//// 어떤 버퍼로 사용되는지
//	ZeroMemory(&SubresourceData, sizeof(D3D11_SUBRESOURCE_DATA));
//	SubresourceData.pSysMem = &ms.at(0);
//	if (FAILED(CreateBufferResult = g_pDevice->CreateBuffer(&BufferDesc, &SubresourceData, &unit->m_pSkinBuffer))) return false;
//	return true;
//}

bool MParser::CreateBuffer(MMesh * Target, vector<DWORD>& index)
{
	if (index.size() == 0)
	{
		return false;
	}
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	D3D11_BUFFER_DESC BufferDesc;				//// 공용 버퍼 데스크
	D3D11_SUBRESOURCE_DATA SubresourceData;		//// 공용 리소스 데이터
	HRESULT CreateBufferResult;					//// 공용 결과값
	//// 인덱스 버퍼 /////////////////////////////////////////////////////////////////////////////////////////////////////
	ZeroMemory(&BufferDesc, sizeof(D3D11_BUFFER_DESC));
	BufferDesc.ByteWidth = sizeof(DWORD) * index.size();			//// 사이즈 (Byte단위)
	BufferDesc.Usage = D3D11_USAGE_DEFAULT;							//// 사용모드 (DEFAULT는 그래픽카드에 생성)
	BufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;					//// 어떤 버퍼로 사용되는지
	ZeroMemory(&SubresourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	SubresourceData.pSysMem = &index.at(0);
	if (FAILED(CreateBufferResult = g_pDevice->CreateBuffer(&BufferDesc, &SubresourceData, &Target->m_pIndexBuffer))) return false;
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	return true;
}

bool MParser::CheckChar(string::iterator data)
{
	if (*data == ' ')	return true;
	return false;
}

bool MParser::Load(M_STR filename, bool isGeo, bool isBone, MSkeleton* skt)
{
	ifstream stream;
	stream.open(filename);
	if (stream.fail() == true)
	{
		return false;
	}
	string word;
	while (!stream.eof())
	{
		getline(stream, word);
		string::iterator start = word.begin();
		for (string::iterator data = word.begin(); data != word.end(); data++)
		{
			if (CheckChar(data))
			{
				string newword(start, data);
				if (newword.size() != 0)
				{
					m_wordlist.push_back(newword);
				}
				start = data + 1;
			}
		}
		string newword(start, word.end());
		if (newword.size() != 0)
		{
			m_wordlist.push_back(newword);
		}
	}
	stream.close();
	TCHAR Drive[MAX_PATH] = { 0, };
	TCHAR Dir[MAX_PATH] = { 0, };
	TCHAR Name[MAX_PATH] = { 0, };
	TCHAR Ext[MAX_PATH] = { 0, };
	_tsplitpath_s(filename.c_str(), Drive, Dir, Name, Ext); // 패스와 이름과 확장자 끊어주기
	M_STR name = Name; // 이름저장
	CreateData(name, isGeo, isBone, skt);
	return true;
}

MFiled* MParser::Load_HM(M_STR filename, float leafsize, float height, int minlevel, float lodstartdistance,bool isChange)
{
	M3DHeightMap* hm = new M3DHeightMap;
	if (hm->Create(filename, leafsize, height, minlevel, lodstartdistance) == false)
	{
		hm->Release();
		delete hm;
		return nullptr;
	}
	hm->Init();
	MFiled* result = I_3DObjectMgr.CreateFiled(hm, isChange);
	if(result == nullptr)
	{
		hm->Release();
		delete hm;
		return nullptr;
	}
	return result;
}

bool MParser::Load_BB(M_STR filename, MUnit* unit, bool isClear)
{
	ifstream stream;
	stream.open(filename);
	if (stream.fail() == true)
	{
		return false;
	}
	string word;
	while (!stream.eof())
	{
		getline(stream, word);
		string::iterator start = word.begin();
		for (string::iterator data = word.begin(); data != word.end(); data++)
		{
			if (CheckChar(data))
			{
				string newword(start, data);
				if (newword.size() != 0)
				{
					m_wordlist.push_back(newword);
				}
				start = data + 1;
			}
		}
		string newword(start, word.end());
		if (newword.size() != 0)
		{
			m_wordlist.push_back(newword);
		}
	}
	stream.close();
	TCHAR Drive[MAX_PATH] = { 0, };
	TCHAR Dir[MAX_PATH] = { 0, };
	TCHAR Name[MAX_PATH] = { 0, };
	TCHAR Ext[MAX_PATH] = { 0, };
	_tsplitpath_s(filename.c_str(), Drive, Dir, Name, Ext); // 패스와 이름과 확장자 끊어주기
	M_STR name = Name; // 이름저장
	SetBBData(name, unit, isClear);
	return false;
}

bool MParser::SetBBData(M_STR name, MUnit* unit, bool isClear)
{
	if (m_wordlist.size() == 0) return false;
	if (unit == nullptr) return false;
	MSkeleton* skt = unit->skt;
	M3DObject* target = nullptr;
	if (isClear)
	{
		SAFE_RELEASE(unit->m_Box);
		SAFE_DELETE(unit->m_Box)
	}
	for (ITOR data = m_wordlist.begin(); data != m_wordlist.end(); data++)
	{
		if (*data == "GEOMETRY")
		{
			data++; data++;
			M_STR oname = DataToMSTR(data);
			target = skt->Find(oname);
		}
		if (*data == "BOUNDING_BOX")
		{
			data++;
			float minx = DataToFloat(data);
			float miny = DataToFloat(data);
			float minz = DataToFloat(data);
			float maxx = DataToFloat(data);
			float maxy = DataToFloat(data);
			float maxz = DataToFloat(data);
			MBoundingBox* box = new MBoundingBox;
			box->Init();
			box->fOldExtent[0] = (maxx - minx) / 2;
			box->fOldExtent[1] = (maxy - miny) / 2;
			box->fOldExtent[2] = (maxz - minz) / 2;
			box->vOldCenter = (D3DXVECTOR3(minx, miny, minz) + D3DXVECTOR3(maxx, maxy, maxz)) / 2;
			box->m_pTarget = target;
			target->m_Box = box;
			unit->m_Box->m_ChildList.push_back(box);
		}
	}
	m_wordlist.clear();
}

bool MParser::CreateData(M_STR name, bool isGeo, bool isBone, MSkeleton* skt2)
{
	if (m_wordlist.size() == 0)
	{
		return false;
	}
	MSkeleton* skt = skt2;
	MKeyAnimation* KeyAni = nullptr;
	if (isBone == true)
	{
		skt = new MSkeleton;
		KeyAni = I_KeyAnimationMgr.NewKeyAnimation(name);
	}
	vector<M_STR> namelist;
	vector<MSkinMesh*> list;
	for (ITOR data = m_wordlist.begin(); data != m_wordlist.end(); data++)
	{
		if (*data == "TYPE")
		{
			data++;
			if (isGeo = true)
			{
				if (*data == "GEOMETRY")
				{
					data++; data++;
					M3DModel* NewModel = new M3DModel;
					NewModel->Init();
					MMesh* NewMeshObj = new MMesh;
					M_STR oname = DataToMSTR(data);
					NewModel->m_name = oname;
					NewModel->m_pObj = I_MeshMgr.Add(NewMeshObj, oname);
					if (NewModel->m_pObj != nullptr)
					{
						I_3DObjectMgr.Add(NewModel);
						CreateGeometryData(data, NewModel, KeyAni, oname);
						namelist.push_back(oname);
					}
					else
					{
						M_STR message;
						message = L"실패. 같은 이름의 오브젝트 : ";
						message += oname;
						MessageBoxW(g_hWnd, (LPCWSTR)&message, L"Error", MB_OK);
						NewMeshObj->Release();
						NewModel->Release();
						delete NewMeshObj;
						delete NewModel;
					}
					data--;
				}
				else if (*data == "SKINGEOMETRY")
				{
					data++; data++;
					MSkinModel* NewUnit = new MSkinModel;
					NewUnit->CreateConstantBuffer();
					MSkinMesh* NewMeshObj = new MSkinMesh;
					M_STR oname = DataToMSTR(data);
					NewUnit->m_name = oname;
					NewUnit->m_pObj = I_MeshMgr.Add(NewMeshObj, oname);
					if (NewUnit->m_pObj != nullptr)
					{
						I_3DObjectMgr.Add(NewUnit);
						CreateSkinningData(data, NewUnit, NewMeshObj);
						list.push_back(NewMeshObj);
						namelist.push_back(oname);
					}
					else
					{
						M_STR message;
						message = L"실패. 같은 이름의 오브젝트 : ";
						message += oname;
						MessageBoxW(g_hWnd, (LPCWSTR)&message, L"Error", MB_OK);
						NewMeshObj->Release();
						NewUnit->Release();
						delete NewMeshObj;
						delete NewUnit;
					}
					data--;
				}
			}
			if (isBone == true)
			{
				if (*data == "BONE")
				{
					data++; data++;
					M_STR oname = DataToMSTR(data);
					M3DBone* temp = skt->NewBone(oname);
					CreateBoneData(oname, data, temp, KeyAni, skt);
					namelist.push_back(oname);
					data--;
				}
			}
		}
	}
	// ----------------------------------------------------
	m_wordlist.clear();
	if (isBone == true)
	{
		if (skt->m_BoneList.size() != 0)
		{
			if (list.size() == 0)
			{
				skt->Release();
				delete skt;
			}
			else
			{
				skt->BindAni(name);
				skt->Set();
				I_SkeletonMgr.Add(skt, name);
				for (MSkinMesh* data : list)
				{
					data->SetSkeletonAndCreateData(skt);
				}
			}
		}
		else
		{
			skt->Release();
			delete skt;
			return true;
		}
	}
	else
	{
		if (skt != nullptr)
		{
			for (MSkinMesh* data : list)
			{
				data->SetSkeletonAndCreateData(skt);
			}
		}
	}
	return true;
}

bool MParser::CreateGeometryData(ITOR &data, M3DModel * target, MKeyAnimation* ani, M_STR name)
{
	vector<MVERTEX>	vertices;
	vector<DWORD>	index;
	while (1)
	{
		if (*data == "PARENT")
		{
			data++;
			if (*data != "NULL")
			{
				M_STR name = DataToMSTR(data);
				target->LinkParents(I_3DObjectMgr[name]);
			}
			data--;
		}
		if (*data == "World_Position")
		{
			data++;
			float x = DataToFloat(data);
			float y = DataToFloat(data);
			float z = DataToFloat(data);
			target->SetLocalPosition(D3DXVECTOR3(x, y, z));
			data--;
		}
		if (*data == "World_Rotation")
		{
			data++;
			float x = DataToFloat(data);
			float y = DataToFloat(data);
			float z = DataToFloat(data);
			float w = DataToFloat(data);
			target->SetLocalRotation(D3DXQUATERNION(x, y, z, w));
			data--;
		}
		if (*data == "World_Scale")
		{
			data++;
			float x = DataToFloat(data);
			float y = DataToFloat(data);
			float z = DataToFloat(data);
			target->SetLocalScale(D3DXVECTOR3(x, y, z));
			data--;
		}
		if (*data == "MVERTEX_COUNT")
		{

			data++;
			int iCount = DataToInt(data);
			for (int i = 0; i < iCount; i++)
			{
				MVERTEX newdata;
				ZeroMemory(&newdata, sizeof(newdata));
				newdata.p.x = DataToFloat(data);
				newdata.p.y = DataToFloat(data);
				newdata.p.z = DataToFloat(data);
				newdata.t.x = DataToFloat(data);
				newdata.t.y = DataToFloat(data);
				newdata.t.z = DataToFloat(data);
				newdata.n.x = DataToFloat(data);
				newdata.n.y = DataToFloat(data);
				newdata.n.z = DataToFloat(data);
				newdata.tv.x = DataToFloat(data);
				newdata.tv.y = DataToFloat(data);
				newdata.tv.z = DataToFloat(data);
				vertices.push_back(newdata);
			}
			data--;
		}
		if (*data == "INDEX_COUNT")
		{
			data++;
			int iCount = DataToInt(data);
			for (int i = 0; i < iCount; i++)
			{
				index.push_back(DataToInt(data));
			}
			data--;
		}
		if (*data == "BOUNDING_BOX")
		{
			data++;
			float minx = DataToFloat(data);
			float miny = DataToFloat(data);
			float minz = DataToFloat(data);
			float maxx = DataToFloat(data);
			float maxy = DataToFloat(data);
			float maxz = DataToFloat(data);
			target->m_Box = new MBoundingBox;
			target->m_Box->Init();
			target->m_Box->fOldExtent[0] = (maxx - minx) / 2;
			target->m_Box->fOldExtent[1] = (maxy - miny) / 2;
			target->m_Box->fOldExtent[2] = (maxz - minz) / 2;
			target->m_Box->vOldCenter = (D3DXVECTOR3(minx, miny, minz) + D3DXVECTOR3(maxx, maxy, maxz)) / 2;
			target->m_Box->m_pTarget = target;
		}
		if (*data == "MAP_ID")
		{
			data++;
			MAPTYPE maptype = DataToMAPTYPE(data);
			M_STR name = DataToMSTR(data);
			target->Load_MAP(name, maptype);
			data--;
		}
		if (*data == "OBJECT_END")
		{
			CreateBuffer(target->m_pObj, vertices, index);
			//target->m_pObj->m_iVertexCount = vertices.size();
			target->m_pObj->m_iIndexCount = index.size();
			vertices.clear();
			index.clear();
			return true;
		}
		data++;
	}
	return false;
}

bool MParser::CreateSkinningData(ITOR &data, MSkinModel* target, MSkinMesh* mesh)
{
	vector<DWORD>	index;
	while (1)
	{
		if (*data == "PARENT")
		{
			data++;
			if (*data != "NULL")
			{
				M_STR name = DataToMSTR(data);
				target->LinkParents(I_3DObjectMgr[name]);
			}
			data--;
		}
		//if (*data == "Skin_Bone_Name")
		//{
		//	data++;
		//	M_STR oname = DataToMSTR(data);
		//	data++;
		//	D3DXVECTOR3 pos;
		//	D3DXQUATERNION rot;
		//	pos.x = DataToFloat(data);
		//	pos.y = DataToFloat(data);
		//	pos.z = DataToFloat(data);
		//	data++;
		//	rot.x = DataToFloat(data);
		//	rot.y = DataToFloat(data);
		//	rot.z = DataToFloat(data);
		//	rot.w = DataToFloat(data);
		//	if (skt != nullptr)
		//	{
		//		skt->AddZeroMat(oname, pos, rot);
		//	}
		//	data--;
		//}
		if (*data == "World_Position")
		{
			data++;
			float x = DataToFloat(data);
			float y = DataToFloat(data);
			float z = DataToFloat(data);
			target->SetLocalPosition(D3DXVECTOR3(x, y, z));
			data--;
		}
		if (*data == "World_Rotation")
		{
			data++;
			float x = DataToFloat(data);
			float y = DataToFloat(data);
			float z = DataToFloat(data);
			float w = DataToFloat(data);
			target->SetLocalRotation(D3DXQUATERNION(x, y, z, w));
			data--;
		}
		if (*data == "World_Scale")
		{
			data++;
			float x = DataToFloat(data);
			float y = DataToFloat(data);
			float z = DataToFloat(data);
			target->SetLocalScale(D3DXVECTOR3(x, y, z));
			data--;
		}
		if (*data == "CVERTEX_COUNT")
		{
			data++;
			int iCount = DataToInt(data);
			for (int i = 0; i < iCount; i++)
			{
				CVERTEX newdata;
				ZeroMemory(&newdata, sizeof(newdata));
				newdata.p.x = DataToFloat(data);
				newdata.p.y = DataToFloat(data);
				newdata.p.z = DataToFloat(data);
				newdata.t.x = DataToFloat(data);
				newdata.t.y = DataToFloat(data);
				newdata.t.z = DataToFloat(data);
				newdata.n.x = DataToFloat(data);
				newdata.n.y = DataToFloat(data);
				newdata.n.z = DataToFloat(data);
				newdata.tv.x = DataToFloat(data);
				newdata.tv.y = DataToFloat(data);
				newdata.tv.z = DataToFloat(data);
				int iNum = DataToInt(data);
				newdata.bp.m_NumWeight = iNum;
				for (int i = 0; i < iNum; i++)
				{
					newdata.bp.m_ID[i] = DataToMSTR(data);
					newdata.bp.m_Weight[i] = DataToFloat(data);
				}
				mesh->m_OldData.push_back(newdata);
			}
			data--;
		}
		if (*data == "INDEX_COUNT")
		{
			data++;
			int iCount = DataToInt(data);
			for (int i = 0; i < iCount; i++)
			{
				index.push_back(DataToInt(data));
			}
			data--;
		}
		if (*data == "MAP_ID")
		{
			data++;
			MAPTYPE maptype = DataToMAPTYPE(data);
			M_STR name = DataToMSTR(data);
			target->Load_MAP(name, maptype);
			data--;
		}
		if (*data == "OBJECT_END")
		{
			CreateBuffer(mesh, index);
			mesh->m_iIndexCount = index.size();
			index.clear();
			return true;
		}
		data++;
	}
	return false;
}

bool MParser::CreateBoneData(M_STR name, ITOR &data, M3DBone* target, MKeyAnimation* KeyAni, MSkeleton* skt)
{
	MKeyData* keydata = new MKeyData;
	while (1)
	{
		if (*data == "PARENT")
		{
			data++;
			if (*data != "NULL")
			{
				M_STR pname = DataToMSTR(data);
				target->LinkParents(skt->Find(pname));
				data--;
			}
		}
		if (*data == "World_Position")
		{
			data++;
			float x = DataToFloat(data);
			float y = DataToFloat(data);
			float z = DataToFloat(data);
			target->SetLocalPosition(D3DXVECTOR3(x, y, z));
			data--;
		}
		if (*data == "World_Rotation")
		{
			data++;
			float x = DataToFloat(data);
			float y = DataToFloat(data);
			float z = DataToFloat(data);
			float w = DataToFloat(data);
			target->SetLocalRotation(D3DXQUATERNION(x, y, z, w));
			data--;
		}
		if (*data == "World_Scale")
		{
			data++;
			float x = DataToFloat(data);
			float y = DataToFloat(data);
			float z = DataToFloat(data);
			target->SetLocalScale(D3DXVECTOR3(x, y, z));
			data--;
		}
		if (*data == "BOUNDING_BOX")
		{
			data++;
			float minx = DataToFloat(data);
			float miny = DataToFloat(data);
			float minz = DataToFloat(data);
			float maxx = DataToFloat(data);
			float maxy = DataToFloat(data);
			float maxz = DataToFloat(data);
			target->m_Box = new MBoundingBox;
			target->m_Box->Init();
			target->m_Box->fOldExtent[0] = (maxx - minx) / 2;
			target->m_Box->fOldExtent[1] = (maxy - miny) / 2;
			target->m_Box->fOldExtent[2] = (maxz - minz) / 2;
			target->m_Box->vOldCenter = (D3DXVECTOR3(minx, miny, minz) + D3DXVECTOR3(maxx, maxy, maxz)) / 2;
			target->m_Box->m_pTarget = target;
		}
		if (*data == "POSITION_KEY_COUNT")
		{
			data++;
			int iCount = DataToInt(data);
			for (int i = 0; i < iCount; i++)
			{
				float t = DataToFloat(data);
				float x = DataToFloat(data);
				float y = DataToFloat(data);
				float z = DataToFloat(data);
				keydata->m_vPositionKeyList.push_back(KEY_Position(t, x, y, z));
			}
			data--;
		}
		if (*data == "ROTATION_KEY_COUNT")
		{
			data++;
			int iCount = DataToInt(data);
			for (int i = 0; i < iCount; i++)
			{
				float t = DataToFloat(data);
				D3DXQUATERNION qRotation;
				qRotation.x = DataToFloat(data);
				qRotation.y = DataToFloat(data);
				qRotation.z = DataToFloat(data);
				qRotation.w = DataToFloat(data);
				keydata->m_vRotationKeyList.push_back(KEY_Rotation(t, qRotation));
			}
			data--;
		}
		if (*data == "SCALE_KEY_COUNT")
		{
			data++;
			int iCount = DataToInt(data);
			for (int i = 0; i < iCount; i++)
			{
				float t = DataToFloat(data);
				float x = DataToFloat(data);
				float y = DataToFloat(data);
				float z = DataToFloat(data);
				keydata->m_vScaleKeyList.push_back(KEY_Scale(t, x, y, z));
			}
			data--;
		}
		if (*data == "OBJECT_END")
		{
			KeyAni->m_List.insert(make_pair(name, keydata));
			return true;
		}
		data++;
	}
	return false;
}