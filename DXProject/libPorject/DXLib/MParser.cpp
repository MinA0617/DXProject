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

M_STR MParser::DataToMSTR(ITOR &data)
{
	string temp = *data;
	M_STR name;
	name.append(temp.begin(), temp.end());
	data++;
	return name;
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

bool MParser::CreateBuffer(MUnit * unit)
{
	vector<MVERTEX>		mv;
	vector<SKINDATA>	ms;
	unit->SetRCVertex(mv, ms);
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	D3D11_BUFFER_DESC BufferDesc;				//// 공용 버퍼 데스크
	D3D11_SUBRESOURCE_DATA SubresourceData;		//// 공용 리소스 데이터
	HRESULT CreateBufferResult;					//// 공용 결과값
	///// 버텍스 버퍼 ///////////////////////////////////////////////////////////////////////////////////////////////////
	ZeroMemory(&BufferDesc, sizeof(D3D11_BUFFER_DESC));
	BufferDesc.ByteWidth = sizeof(MVERTEX) * mv.size();		//// 사이즈 (Byte단위)
	BufferDesc.Usage = D3D11_USAGE_DEFAULT;							//// 사용모드 (DEFAULT는 그래픽카드에 생성)
	BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;				//// 어떤 버퍼로 사용되는지
	ZeroMemory(&SubresourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	SubresourceData.pSysMem = &mv.at(0);
	if (FAILED(CreateBufferResult = g_pDevice->CreateBuffer(&BufferDesc, &SubresourceData, &I_MeshMgr[unit->ObjID]->m_pVertexBuffer))) return false;
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///// 버텍스 버퍼 ///////////////////////////////////////////////////////////////////////////////////////////////////
	ZeroMemory(&BufferDesc, sizeof(D3D11_BUFFER_DESC));
	BufferDesc.ByteWidth = sizeof(SKINDATA) * ms.size();		//// 사이즈 (Byte단위)
	BufferDesc.Usage = D3D11_USAGE_DEFAULT;							//// 사용모드 (DEFAULT는 그래픽카드에 생성)
	BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;				//// 어떤 버퍼로 사용되는지
	ZeroMemory(&SubresourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	SubresourceData.pSysMem = &ms.at(0);
	if (FAILED(CreateBufferResult = g_pDevice->CreateBuffer(&BufferDesc, &SubresourceData, &unit->m_pSkinBuffer))) return false;
}

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

bool MParser::CreateBuffer(M3DBone * Target)
{
	SPLINE_VERTEX data[6];
	ZeroMemory(&data, sizeof(SPLINE_VERTEX) * 6);
	data[0].p = float3(10, 0, 0);
	data[1].p = float3(-10, 0, 0);
	data[2].p = float3(0, 10, 0);
	data[3].p = float3(0, -10, 0);
	data[4].p = float3(0, 0, 10);
	data[5].p = float3(0, 0, -10);
	DWORD sindex[6];
	for (int i = 0; i < 6; i++)
	{
		sindex[i] = i;
	}
	Target->m_iIndexCount = 6;
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	D3D11_BUFFER_DESC BufferDesc;				//// 공용 버퍼 데스크
	D3D11_SUBRESOURCE_DATA SubresourceData;		//// 공용 리소스 데이터
	HRESULT CreateBufferResult;					//// 공용 결과값
	///// 버텍스 버퍼 ///////////////////////////////////////////////////////////////////////////////////////////////////
	ZeroMemory(&BufferDesc, sizeof(D3D11_BUFFER_DESC));
	BufferDesc.ByteWidth = sizeof(SPLINE_VERTEX) * 6;		//// 사이즈 (Byte단위)
	BufferDesc.Usage = D3D11_USAGE_DEFAULT;							//// 사용모드 (DEFAULT는 그래픽카드에 생성)
	BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;				//// 어떤 버퍼로 사용되는지
	ZeroMemory(&SubresourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	SubresourceData.pSysMem = &data;
	if (FAILED(CreateBufferResult = g_pDevice->CreateBuffer(&BufferDesc, &SubresourceData, &Target->m_pVertexBuffer))) return false;
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//// 인덱스 버퍼 /////////////////////////////////////////////////////////////////////////////////////////////////////
	ZeroMemory(&BufferDesc, sizeof(D3D11_BUFFER_DESC));
	BufferDesc.ByteWidth = sizeof(DWORD) * 6;			//// 사이즈 (Byte단위)
	BufferDesc.Usage = D3D11_USAGE_DEFAULT;							//// 사용모드 (DEFAULT는 그래픽카드에 생성)
	BufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;					//// 어떤 버퍼로 사용되는지
	ZeroMemory(&SubresourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	SubresourceData.pSysMem = &sindex;
	if (FAILED(CreateBufferResult = g_pDevice->CreateBuffer(&BufferDesc, &SubresourceData, &Target->m_pIndexBuffer))) return false;
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	return true;
}

bool MParser::CheckChar(string::iterator data)
{
	if (*data == ' ')	return true;
	return false;
}

bool MParser::Load(M_STR filename)
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
	name += Ext; // 확장자 추가해서 저장
	CreateData(name);
	return true;
}

bool MParser::CreateData(M_STR name)
{
	if (m_wordlist.size() == 0)
	{
		return false;
	}
	MSkeleton* skt = new MSkeleton;
	MKeyAnimation* KeyAni = I_KeyAnimationMgr.NewKeyAnimation(name);
	vector<M_STR> namelist;
	vector<MUnit*> list;
	for (ITOR data = m_wordlist.begin(); data != m_wordlist.end(); data++)
	{
		if (*data == "TYPE")
		{
			data++;
			if (*data == "GEOMETRY")
			{
				data++; data++;
				M3DModel* NewModel = new M3DModel;
				NewModel->CreateConstantBuffer();
				MMesh* NewMeshObj = new MMesh;
				M_STR oname = DataToMSTR(data);
				NewModel->ObjID = I_MeshMgr.Add(NewMeshObj, oname);
				I_3DObjectMgr.Add(oname, NewModel);
				CreateGeometryData(data, NewModel, KeyAni, oname);
				namelist.push_back(oname);
				data--;
			}
			else if (*data == "SKINGEOMETRY")
			{
				data++; data++;
				MUnit* NewUnit = new MUnit;
				NewUnit->CreateConstantBuffer();
				MMesh* NewMeshObj = new MMesh;
				M_STR oname = DataToMSTR(data);
				list.push_back(NewUnit);
				NewUnit->ObjID = I_MeshMgr.Add(NewMeshObj, oname);
				I_3DObjectMgr.Add(oname, NewUnit);
				CreateSkinningData(data, NewUnit);
				namelist.push_back(oname);
				data--;
			}
			else if (*data == "BONE")
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
	// ----------------------------------------------------
	if (skt->m_BoneList.size() != 0)
	{
		skt->BindAni(KeyAni);
		skt->Set();
		I_SkeletonMgr.Add(skt);
		for (MUnit* data : list)
		{
			data->Init();
			data->m_Skeleton = skt;
			CreateBuffer(data);
		}
	}
	else
	{
		delete skt;
	}
	return true;
}

bool MParser::CreateGeometryData(ITOR &data, M3DModel * target, MKeyAnimation* ani, M_STR name)
{
	MKeyData* keydata = new MKeyData;
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
			CreateBuffer(I_MeshMgr[target->ObjID], vertices, index);
			I_MeshMgr[target->ObjID]->m_iVertexCount = vertices.size();
			I_MeshMgr[target->ObjID]->m_iIndexCount = index.size();
			vertices.clear();
			index.clear();
			if (keydata->isUse())
			{
				target->m_KeyData = keydata;
				ani->m_List.insert(make_pair(name, keydata));
			}
			else
			{
				delete keydata;
			}
			return true;
		}
		data++;
	}
	return false;
}

bool MParser::CreateSkinningData(ITOR &data, MUnit* target)
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
				target->m_OldData.push_back(newdata);
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
			CreateBuffer(I_MeshMgr[target->ObjID], index);
			I_MeshMgr[target->ObjID]->m_iIndexCount = index.size();
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
	CreateBuffer(target);
	while (1)
	{
		if (*data == "PARENT")
		{
			data++;
			if (*data != "NULL")
			{
				M_STR pname = DataToMSTR(data);
				target->LinkParents((*(skt->m_BoneList.find(pname))).second);
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
		if (*data == "OBJECT_END")
		{
			KeyAni->m_List.insert(make_pair(name, keydata));
			return true;
		}
		data++;
	}
	return false;
}