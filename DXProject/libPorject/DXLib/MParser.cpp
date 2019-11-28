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
	case 6:
		return MAPTYPE(OPACITY);
		break;
	default:
		return MAPTYPE(NOTFOUND);
		break;
	}
}

M_STR MParser::DataToMSTR(ITOR & data)
{
	string temp = *data;
	data++;
	M_STR mstr;
	mstr.append(temp.begin(), temp.end());
	return mstr;
}

D3DXVECTOR3 MParser::FindMin(vector<MVERTEX>& vertices)
{
	int size = vertices.size();
	if (size == 0)
	{
		return D3DXVECTOR3(0, 0, 0);
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
	return D3DXVECTOR3(x, y, z);
}

D3DXVECTOR3 MParser::FindMax(vector<MVERTEX>& vertices)
{
	int size = vertices.size();
	if (size == 0)
	{
		return D3DXVECTOR3(0, 0, 0);
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
	return D3DXVECTOR3(x, y, z);
}

M_STR MParser::AnotherPath(M_STR oldpath)
{
	M_STR str = oldpath;
	str += L" 파일을 찾을 수 없습니다. 경로를 재설정 해주십시오.";
	MessageBoxW(g_hWnd, str.c_str(), L"Error", MB_OK);
	OPENFILENAME    ofn;
	TCHAR           szFile[MAX_PATH] = { 0, };
	TCHAR			szFileTitle[MAX_PATH] = { 0, };
	static TCHAR    *szFilter;

	TCHAR lpCurBuffer[256] = { 0, };
	GetCurrentDirectory(256, lpCurBuffer);

	ZeroMemory(&ofn, sizeof(OPENFILENAME));
	_tcscpy_s(szFile, _T("*.*\0"));

	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = GetActiveWindow();
	ofn.lpstrFilter = szFilter;
	ofn.lpstrCustomFilter = NULL;
	ofn.nMaxCustFilter = 0L;
	ofn.nFilterIndex = 1;
	ofn.lpstrFile = szFile;
	ofn.nMaxFile = MAX_PATH;
	ofn.lpstrFileTitle = szFileTitle;
	ofn.nMaxFileTitle = MAX_PATH;
	ofn.lpstrInitialDir = _T("../../data/");
	ofn.lpstrTitle = L"asd";
	ofn.Flags = OFN_EXPLORER | OFN_ALLOWMULTISELECT;
	ofn.nFileOffset = 0;
	ofn.nFileExtension = 0;

	if (!GetOpenFileName(&ofn))
	{
		return false;
	}
	TCHAR* load = _tcstok(szFile, _T("\n"));
	M_STR dir = szFile;

	SetCurrentDirectory(lpCurBuffer);
	return dir;
}

bool MParser::Load_Map(M3DInstanceModel* target, M_STR name, MAPTYPE maptype)
{
	if (!target->Load_MAP(name, maptype))
	{
		while (1)
		{
			M_STR newname = AnotherPath(name);
			if (target->Load_MAP(newname, maptype))
			{
				break;
			}
		}
	}
	return true;
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
	Target->m_iIndexCount = index.size();
	return true;
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

bool MParser::CheckChar(string::iterator data)
{
	if (*data == ' ')	return true;
	return false;
}

bool MParser::CreateMeshData(ITOR & data, M3DInstanceModel * target)
{
	vector<MVERTEX>	vertices;
	vector<DWORD>	index;
	while (1)
	{
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
#if defined(DEBUG) || defined(_DEBUG)
			target->m_Box.Init();
#endif // DEBUG
			target->m_Box.fOldExtent[0] = (maxx - minx) / 2;
			target->m_Box.fOldExtent[1] = (maxy - miny) / 2;
			target->m_Box.fOldExtent[2] = (maxz - minz) / 2;
			target->m_Box.vOldCenter = (D3DXVECTOR3(minx, miny, minz) + D3DXVECTOR3(maxx, maxy, maxz)) / 2;
		}
		if (*data == "MAP_ID")
		{
			data++;
			MAPTYPE maptype = DataToMAPTYPE(data);
			M_STR name = DataToMSTR(data);
			Load_Map(target, name, maptype);
			data--;
		}
		if (*data == "OBJECT_END")
		{
			CreateBuffer(target->m_Mesh, vertices, index);
			vertices.clear();
			index.clear();
			return true;
		}
		data++;
	}
	m_wordlist.clear();
	return true;
}


bool MParser::ParsingDate(M_STR filename, vector<string>& list)
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
					list.push_back(newword);
				}
				start = data + 1;
			}
		}
		string newword(start, word.end());
		if (newword.size() != 0)
		{
			list.push_back(newword);
		}
	}
	stream.close();
	return true;
}

bool MParser::Load(M_STR filename, MSkeleton* target)
{
	int flag = 0;
	TCHAR Drive[MAX_PATH] = { 0, };
	TCHAR Dir[MAX_PATH] = { 0, };
	TCHAR Name[MAX_PATH] = { 0, };
	TCHAR Ext[MAX_PATH] = { 0, };
	_tsplitpath_s(filename.c_str(), Drive, Dir, Name, Ext); // 패스와 이름과 확장자 끊어주기
	M_STR name = Name; // 이름저장
	M_STR ext = Ext;
	if (!ext.compare(L".OBJ")) { flag = 1; }
	else if (!ext.compare(L".SKT")) { flag = 2; }
	else if (!ext.compare(L".KEY")) { flag = 3; }
	else { return false; }

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
	switch (flag)
	{
	case 1:
	{
		CreateOBJData(name, target);
		break;
	}
	case 2:
	{
		CreateSKTData(name);
		break;
	}
	case 3:
	{
		CreateKEYData(name);
		break;
	}
	default:
		break;
	}
	return true;
}

bool MParser::Load(M_STR filename, vector<M_STR>& namelist)
{
	vector<string> wordlist;
	ParsingDate(filename, wordlist);
	CreateInstanceOBJData(wordlist, namelist);
	if (namelist.size() == 0) return false;
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

bool MParser::Load_BB(M_STR filename, MSkeleton* skt, bool isClear)
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
	SetBBData(name, skt, isClear);
	return false;
}

bool MParser::SetBBData(M_STR name, MSkeleton* skt, bool isClear)
{
	if (m_wordlist.size() == 0) return false;
	if (skt == nullptr) return false;
	M3DBone* target = nullptr;
	if (isClear)
	{
#if defined(DEBUG) || defined(_DEBUG)
		SAFE_RELEASE(skt->m_Box);
#endif // DEBUG
		SAFE_DELETE(skt->m_Box)
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
#if defined(DEBUG) || defined(_DEBUG)
			box->Init();
#endif // DEBUG
			box->fOldExtent[0] = (maxx - minx) / 2;
			box->fOldExtent[1] = (maxy - miny) / 2;
			box->fOldExtent[2] = (maxz - minz) / 2;
			box->vOldCenter = (D3DXVECTOR3(minx, miny, minz) + D3DXVECTOR3(maxx, maxy, maxz)) / 2;
			target->m_Box = box;
		}
	}
	m_wordlist.clear();
}

bool MParser::CreateInstanceOBJData(vector<string>& list, vector<M_STR>& namelist)
{
	if (!list.size()) return false;
	for (ITOR data = list.begin(); data != list.end(); data++)
	{
		if (*data == "TYPE")
		{
			data++;
			if (*data == "GEOMETRY")
			{
				data++; data++;
				M3DInstanceModel* NewModel = new M3DInstanceModel;
				NewModel->Init();
				MMesh* NewMeshObj = new MMesh;
				M_STR oname = DataToMSTR(data);
				namelist.push_back(oname);
				NewModel->m_name = oname;
				NewModel->m_Mesh = I_MeshMgr.Add(NewMeshObj, oname);
				NewModel->m_Mesh->m_VertexShaderID = VS3DINSTANCE;
				if (NewModel->m_Mesh != nullptr)
				{
					I_3DObjectMgr.AddInstanceModel(NewModel);
					CreateMeshData(data, NewModel);
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
			//else if (*data == "SKINGEOMETRY")
			//{
			//	//data++; data++;
			//	//MSkinModel* NewSkinModel = new MSkinModel;
			//	//NewSkinModel->CreateConstantBuffer();
			//	//MSkinMesh* NewMeshObj = new MSkinMesh;
			//	//M_STR oname = DataToMSTR(data);
			//	//NewSkinModel->m_name = oname;
			//	//NewSkinModel->m_pObj = I_MeshMgr.Add(NewMeshObj, oname);
			//	//if (NewSkinModel->m_pObj != nullptr)
			//	//{
			//	//	I_3DObjectMgr.Add(NewSkinModel);
			//	//	CreateSkinningData(data, NewSkinModel, NewMeshObj);
			//	//	list.push_back(NewSkinModel);
			//	//	list2.push_back(NewMeshObj);
			//	//}
			//	//else
			//	//{
			//	//	M_STR message;
			//	//	message = L"실패. 같은 이름의 오브젝트 : ";
			//	//	message += oname;
			//	//	MessageBoxW(g_hWnd, (LPCWSTR)&message, L"Error", MB_OK);
			//	//	NewMeshObj->Release();
			//	//	NewSkinModel->Release();
			//	//	delete NewMeshObj;
			//	//	delete NewSkinModel;
			//	//}
			//	//data--;
			//}
		}
	}
	return true;
}

bool MParser::CreateOBJData(M_STR name, MSkeleton* skt)
{
	if (m_wordlist.size() == 0)
	{
		return false;
	}
	vector<MSkinModel*>	list;
	vector<MSkinMesh*>	list2;
	for (ITOR data = m_wordlist.begin(); data != m_wordlist.end(); data++)
	{
		if (*data == "TYPE")
		{
			data++;
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
						CreateGeometryData(data, NewModel);
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
				MSkinModel* NewSkinModel = new MSkinModel;
				NewSkinModel->CreateConstantBuffer();
				MSkinMesh* NewMeshObj = new MSkinMesh;
				M_STR oname = DataToMSTR(data);
				NewSkinModel->m_name = oname;
				NewSkinModel->m_pObj = I_MeshMgr.Add(NewMeshObj, oname);
				if (NewSkinModel->m_pObj != nullptr)
				{
					I_3DObjectMgr.Add(NewSkinModel);
					CreateSkinningData(data, NewSkinModel, NewMeshObj);
					list.push_back(NewSkinModel);
					list2.push_back(NewMeshObj);
				}
				else
				{
					M_STR message;
					message = L"실패. 같은 이름의 오브젝트 : ";
					message += oname;
					MessageBoxW(g_hWnd, (LPCWSTR)&message, L"Error", MB_OK);
					NewMeshObj->Release();
					NewSkinModel->Release();
					delete NewMeshObj;
					delete NewSkinModel;
				}
				data--;
			}
		}
	}
	// ----------------------------------------------------
	m_wordlist.clear();
	for (int i = 0; i < list.size(); i++)
	{
		list[i]->m_Skeleton = skt;
		list2[i]->SetSkeletonAndCreateData(skt);
	}
	return true;
}

bool MParser::CreateSKTData(M_STR name)
{
	if (m_wordlist.size() == 0) { return false; }
	MSkeleton* skt = new MSkeleton;
	I_SkeletonMgr.Add(skt, name);
	M3DBone* target = nullptr;
	for (ITOR data = m_wordlist.begin(); data != m_wordlist.end(); data++)
	{
		if (*data == "OBJECT_NAME")
		{
			data++;
			M_STR bname = DataToMSTR(data);
			M3DBone* newbone = skt->NewBone(bname);
			target = newbone;
		}
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
#if defined(DEBUG) || defined(_DEBUG)
			target->m_Box->Init();
#endif // DEBUG
			target->m_Box->fOldExtent[0] = (maxx - minx) / 2;
			target->m_Box->fOldExtent[1] = (maxy - miny) / 2;
			target->m_Box->fOldExtent[2] = (maxz - minz) / 2;
			target->m_Box->vOldCenter = (D3DXVECTOR3(minx, miny, minz) + D3DXVECTOR3(maxx, maxy, maxz)) / 2;
		}
		//if (*data == "OBJECT_END")
		//{
		//	skt->AddZeroMat(target);
		//	target->SetZero();
		//}
	}
	skt->Init();
	skt->Set();
	m_wordlist.clear();
	//skt->SetZeroMat();
	return true;
}

bool MParser::CreateKEYData(M_STR name)
{
	if (m_wordlist.size() == 0) { return false; }
	MKeyAnimation* any = I_KeyAnimationMgr.NewKeyAnimation(name);
	if (any == nullptr) { return false; }
	MKeyData* key = nullptr;
	for (ITOR data = m_wordlist.begin(); data != m_wordlist.end(); data++)
	{
		if (*data == "OBJECT_NAME")
		{
			data++;
			M_STR anyname = DataToMSTR(data);
			MKeyData* newkey = any->NewKey(anyname);
			key = newkey;
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
				key->m_vPositionKeyList.push_back(KEY_Position(t, x, y, z));
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
				key->m_vRotationKeyList.push_back(KEY_Rotation(t, qRotation));
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
				key->m_vScaleKeyList.push_back(KEY_Scale(t, x, y, z));
			}
			data--;
		}
	}
	m_wordlist.clear();
	return true;
}

bool MParser::CreateGeometryData(ITOR &data, M3DModel* target)
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
#if defined(DEBUG) || defined(_DEBUG)
			target->m_Box->Init();
#endif // DEBUG
			target->m_Box->fOldExtent[0] = (maxx - minx) / 2;
			target->m_Box->fOldExtent[1] = (maxy - miny) / 2;
			target->m_Box->fOldExtent[2] = (maxz - minz) / 2;
			target->m_Box->vOldCenter = (D3DXVECTOR3(minx, miny, minz) + D3DXVECTOR3(maxx, maxy, maxz)) / 2;
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
	m_wordlist.clear();
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