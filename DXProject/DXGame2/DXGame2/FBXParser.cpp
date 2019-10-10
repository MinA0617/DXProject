#include "FBXParser.h"



FBXParser::FBXParser()
{
}


FBXParser::~FBXParser()
{
}

long long FBXParser::DataToLong(ITOR & data)
{
	string temp = *data;
	long long l = atoll(temp.c_str());
	data++;
	return l;
}

int FBXParser::DataToInt(ITOR &data)
{
	string temp = *data;
	int i = atoi(temp.c_str());
	data++;
	return i;
}

float FBXParser::DataToFloat(ITOR &data)
{
	string temp = *data;
	float f = atoi(temp.c_str());
	data++;
	return f;
}

bool FBXParser::CheckChar(string::iterator data)
{
	if (*data == ',')	return true;
	if (*data == ' ')	return true;
	if (*data == '"')	return true;
	if (*data == '*')	return true;
	if (*data == ':')	return true;
	if (*data == '{')	return true;
	if (*data == '}')	return true;
	if (*data == ';')	return true;
	if (*data == '\t')	return true;
	return false;
}

bool FBXParser::Load(M_STR filename, FBXTYPE type)
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
	switch (type)
	{
	case MESH:
		CreateMeshData(name);
		break;
	case SKINNING_MESH:
		CreateSkinnigMeshData(name);
		break;
	case ANIMAITION:
		CreateAnimationData(name);
		break;
	default:
		break;
	}
}

bool FBXParser::CreateMeshData(M_STR name)
{
	if (m_wordlist.size() == 0)
	{
		return false;
	}

	MKeyAnimation*	m_pTargetAni = nullptr;		// 데이터를 쓰게 될 타켓
	M3DBone*		m_pTargetBone = nullptr;		// 데이터를 쓰게 될 타겟


	int iNumVertices = 0;
	int iNumFaces = 0;
	map<int, float3> vertices;
	map<int, float3> texcoords;
	map<int, float3> normals;
	//vector<int3> faces;
	vector<long long> List_id;
	vector<vector<float3>>  List_v;
	vector<vector<int3>> List_vi;
	for (ITOR data = m_wordlist.begin(); data != m_wordlist.end(); data++)
	{
		if (*data == "Geometry")
		{
			data++;
			if ((*data).length() == 13)
			{
				long long temp = DataToLong(data);
				List_id.push_back(temp);
			}
		}
		if (*data == "Vertices")
		{
			// 버텍스 갯수
			data++;
			int iCount = DataToInt(data) / 3;
			data++;
			vector<float3> templist;
			for (int i = 0; i < iCount; i++)
			{
				float3 temp;
				temp.x = DataToFloat(data);
				temp.y = DataToFloat(data);
				temp.z = DataToFloat(data);
				templist.push_back(temp);
			}
			List_v.push_back(templist);
		}
		if (*data == "PolygonVertexIndex")
		{
			// 버텍스 갯수
			data++;
			int iCount = DataToInt(data) / 3;
			data++;
			vector<int3> templist;
			for (int i = 0; i < iCount; i++)
			{
				int3 temp;
				temp.v1 = DataToInt(data);
				temp.v2 = DataToInt(data);
				temp.v3 = DataToInt(data);
				templist.push_back(temp);
			}
			List_vi.push_back(templist);
		}
		if (*data == "PolygonVertexIndex")
		{
			// 버텍스 갯수
			data++;
			int iCount = DataToInt(data) / 3;
			data++;
			vector<int3> templist;
			for (int i = 0; i < iCount; i++)
			{
				int3 temp;
				temp.v1 = DataToInt(data);
				temp.v2 = DataToInt(data);
				temp.v3 = DataToInt(data);
				templist.push_back(temp);
			}
			List_vi.push_back(templist);
		}
	}
	return true;
}
bool FBXParser::CreateSkinnigMeshData(M_STR name)
{
	return false;
}
bool FBXParser::CreateAnimationData(M_STR name)
{
	typedef vector<string>::iterator ITER;
	if (m_wordlist.size() == 0)
	{
		return false;
	}

	MKeyAnimation*	m_pTargetAni = nullptr;		// 데이터를 쓰게 될 타켓
	M3DBone*		m_pTargetBone = nullptr;		// 데이터를 쓰게 될 타겟


	int iNumVertices = 0;
	int iNumFaces = 0;
	map<int, float3> vertices;
	map<int, float3> texcoords;
	map<int, float3> normals;
	vector<int3> faces;


	for (ITER data = m_wordlist.begin(); data != m_wordlist.end(); data++)
	{
		if (*data == "*SCENE_FILENAME")
		{
			// 애니메이션 이름을 가지고 애니메이션 생성
			data++;
			m_pTargetAni = I_KeyAnimationMgr.NewKeyAnimation(name);
		}
		if (*data == "*SCENE_FIRSTFRAME")
		{
			// 시작 프레임
			if (m_pTargetAni != nullptr)
			{
				data++;
				string temp = *data;
				int i = atoi(temp.c_str());
				m_pTargetAni->m_iStartFrame = i;
			}
		}
		if (*data == "*SCENE_LASTFRAME")
		{
			// 마지막 프레임
			if (m_pTargetAni != nullptr)
			{
				data++;
				string temp = *data;
				int i = atoi(temp.c_str());
				m_pTargetAni->m_iLastFrame = i;
			}
		}
		if (*data == "*SCENE_FRAMESPEED ")
		{
			if (m_pTargetAni != nullptr)
			{
				data++;
				string temp = *data;
				int i = atoi(temp.c_str());
				m_pTargetAni->m_iAniFPS = i;
			}
			// 프레임 퍼 세컨드
		}
		if (*data == "*SCENE_TICKSPERFRAME")
		{
			if (m_pTargetAni != nullptr)
			{
				data++;
				string temp = *data;
				int i = atoi(temp.c_str());
				m_pTargetAni->m_i_TickPerFrame = i;
			}
			// 틱 퍼 프레임
		}
		if (*data == "*NODE_NAME")
		{
			// 노드 네임이 나오면 밑의 것들의 쓰는 대상을 바꿈. 중복체크하여 새로운 노드라면 추가시킨다 
			if (m_pTargetAni != nullptr)
			{
				data++;
				string temp = *data;
				M_STR name;
				name.append(temp.begin(), temp.end());
				m_pTargetBone = m_pTargetAni->m_TempSkeleton.NewBone(name);
				// 로테이션키 최초값 넣기
			}

		}
		if (*data == "*TM_POS")
		{
			if (m_pTargetAni != nullptr)
			{
				data++;
				string temp = *data;
				temp = *data;
				float x = atof(temp.c_str());
				data++;
				temp = *data;
				float y = atof(temp.c_str());
				data++;
				temp = *data;
				float z = atof(temp.c_str());
				m_pTargetBone->SetLocalPosition(D3DXVECTOR3(x, y, z));
			}
		}
		if (*data == "*TM_ROTAXIS")
		{
			if (m_pTargetAni != nullptr)
			{
				data++;
				string temp = *data;
				temp = *data;
				float x = atof(temp.c_str());
				data++;
				temp = *data;
				float y = atof(temp.c_str());
				data++;
				temp = *data;
				float z = atof(temp.c_str());
				data += 2;
				temp = *data;
				float w = atof(temp.c_str());
				D3DXQUATERNION rot;
				D3DXQuaternionRotationAxis(&rot, &D3DXVECTOR3(x, y, z), w);
				m_pTargetBone->SetLocalRotation(rot);
			}
		}
		if (*data == "*TM_SCALE")
		{
			if (m_pTargetAni != nullptr)
			{
				data++;
				string temp = *data;
				temp = *data;
				float x = atof(temp.c_str());
				data++;
				temp = *data;
				float y = atof(temp.c_str());
				data++;
				temp = *data;
				float z = atof(temp.c_str());
				m_pTargetBone->SetLocalScale(D3DXVECTOR3(x, y, z));
			}
		}
		if (*data == "*CONTROL_POS_SAMPLE")
		{
			if (m_pTargetBone != nullptr)
			{
				data++;
				string temp = *data;
				float t = atof(temp.c_str()) / 4800;
				data++;
				temp = *data;
				float x = atof(temp.c_str());
				data++;
				temp = *data;
				float y = atof(temp.c_str());
				data++;
				temp = *data;
				float z = atof(temp.c_str());
				m_pTargetBone->m_KeyData.m_vPositionKeyList.push_back(KEY_Position(t, x, y, z));
			}
		}
		if (*data == "*CONTROL_ROT_SAMPLE")
		{
			if (m_pTargetBone != nullptr)
			{
				data++;
				string temp = *data;
				float t = atof(temp.c_str()) / 4800;
				data++;
				temp = *data;
				float x = -atof(temp.c_str());
				data++;
				temp = *data;
				float y = atof(temp.c_str());
				data++;
				temp = *data;
				float z = -atof(temp.c_str());
				data++;
				temp = *data;
				float w = atof(temp.c_str());
				D3DXQUATERNION qRotation;
				D3DXQuaternionRotationAxis(&qRotation, &D3DXVECTOR3(x, y, z), w);
				qRotation *= (*(m_pTargetBone->m_KeyData.m_vRotationKeyList.end() - 1)).data;
				m_pTargetBone->m_KeyData.m_vRotationKeyList.push_back(KEY_Rotation(t, qRotation));
			}
		}
		if (*data == "*CONTROL_SCALE_SAMPLE")
		{
			if (m_pTargetBone != nullptr)
			{
				data++;
				string temp = *data;
				float t = atof(temp.c_str()) / 4800;
				data++;
				temp = *data;
				float x = atof(temp.c_str());
				data++;
				temp = *data;
				float y = atof(temp.c_str());
				data++;
				temp = *data;
				float z = atof(temp.c_str());
				m_pTargetBone->m_KeyData.m_vScaleKeyList.push_back(KEY_Scale(t, x, y, z));
			}
		}
		if (*data == "*MESH_NUMVERTEX")
		{
			data++;
			string temp = *data;
			iNumVertices = atoi(temp.c_str());
		}
		if (*data == "*MESH_NUMFACES")
		{
			data++;
			string temp = *data;
			iNumFaces = atoi(temp.c_str());
		}
		if (*data == "*MESH_VERTEX")
		{
			if (m_pTargetAni != nullptr)
			{
				// 버텍스
				data++;
				string temp = *data;
				int index = atoi(temp.c_str());
				data++;
				temp = *data;
				float x = atof(temp.c_str());
				data++;
				temp = *data;
				float y = atof(temp.c_str());
				data++;
				temp = *data;
				float z = atof(temp.c_str());
				D3DXVECTOR3 ver(x, y, z);
				D3DXMATRIX posmat = m_pTargetBone->GetPositionMatrix();
				D3DXMATRIX rotmat = m_pTargetBone->GetRotationMatrix();;
				D3DXMATRIX sclmat = m_pTargetBone->GetScaleMatrix();;
				D3DXMatrixInverse(&posmat, NULL, &posmat);
				D3DXMatrixInverse(&rotmat, NULL, &rotmat);
				D3DXMatrixInverse(&sclmat, NULL, &sclmat);
				D3DXVec3TransformCoord(&ver, &ver, &posmat);
				//D3DXVec3TransformCoord(&ver, &ver, &rotmat);
				D3DXVec3TransformCoord(&ver, &ver, &sclmat);
				//D3DXVECTOR3 pos = m_pTargetBone->GetPosition();
				//D3DXQUATERNION rot = m_pTargetBone->GetRotation();
				//D3DXVECTOR3 scl = m_pTargetBone->GetScale();
				//D3DXMATRIX mat;
				//D3DXMatrixTransformation(&mat, &pos, &rot, &scl, &pos, &rot, &pos);
				//D3DXMatrixInverse(&mat, NULL, &mat);
				//D3DXVec3TransformCoord(&ver, &ver, &mat);
				vertices.insert(std::make_pair(index, float3(ver.x, ver.y, ver.z)));
			}
		}
		if (*data == "*MESH_VERTEXNORMAL")
		{
			if (m_pTargetAni != nullptr)
			{
				// 노말
				data++;
				string temp = *data;
				int index = atoi(temp.c_str());
				data++;
				temp = *data;
				float x = atof(temp.c_str());
				data++;
				temp = *data;
				float y = atof(temp.c_str());
				data++;
				temp = *data;
				float z = atof(temp.c_str());
				if (normals.find(index) == normals.end())
				{
					normals.insert(std::make_pair(index, float3(x, y, z)));
				}
			}
		}
		if (*data == "*MESH_FACE")
		{
			if (m_pTargetAni != nullptr)
			{
				// 페이스
				data += 3;
				string temp = *data;
				int index1 = atoi(temp.c_str());
				data += 2;
				temp = *data;
				int index2 = atoi(temp.c_str());
				data += 2;
				temp = *data;
				int index3 = atoi(temp.c_str());
				faces.push_back(int3(index1, index2, index3));
			}
		}
		if (*data == "*NODE_PARENT")
		{
			if (m_pTargetAni != nullptr)
			{
				//data++;
				//string temp = *data;
				//M_STR name;
				//name.append(temp.begin(), temp.end());
				//M3DObject* parents = (*(m_pTargetAni->m_TempSkeleton.m_BoneList.find(name))).second;
				//m_pTargetBone->LinkParents(parents);
			}
		}
#pragma region CreateBuffer
		if (iNumVertices == vertices.size() &&
			iNumVertices == normals.size() &&
			iNumFaces == faces.size() &&
			iNumVertices != 0 &&
			iNumFaces != 0)
		{
			// 숫자와 맞을경우 Obj생성
			//vector<P3VERTEX> vertices3D;
			//vector<DWORD> index3D;
			P3VERTEX* vertices3D = new P3VERTEX[iNumVertices];
			DWORD* index3D = new DWORD[iNumFaces * 3];
			for (int i = 0; i < iNumVertices; i++)
			{
				P3VERTEX newdata;
				newdata.p = vertices[i];
				newdata.n = normals[i];
				newdata.t = float3(1, 0, 0);
				vertices3D[i] = newdata;
			}
			for (int i = 0; i < iNumFaces; i++)
			{
				//index3D.push_back(faces[i].v1);
				//index3D.push_back(faces[i].v2);
				//index3D.push_back(faces[i].v3);
				index3D[i * 3 + 0] = faces[i].v1;
				index3D[i * 3 + 1] = faces[i].v3;
				index3D[i * 3 + 2] = faces[i].v2;
			}
			//m_pTargetBone->m_iVertexCount = vertices3D.size();
			//m_pTargetBone->m_iIndexCount = index3D.size();
			m_pTargetBone->m_iVertexCount = iNumVertices;
			m_pTargetBone->m_iIndexCount = iNumFaces * 3;
			//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			D3D11_BUFFER_DESC BufferDesc;				//// 공용 버퍼 데스크
			D3D11_SUBRESOURCE_DATA SubresourceData;		//// 공용 리소스 데이터
			HRESULT CreateBufferResult;					//// 공용 결과값
			///// 버텍스 버퍼 ///////////////////////////////////////////////////////////////////////////////////////////////////
			ZeroMemory(&BufferDesc, sizeof(D3D11_BUFFER_DESC));
			BufferDesc.ByteWidth = sizeof(P3VERTEX) * m_pTargetBone->m_iVertexCount;	//// 사이즈 (Byte단위)
			BufferDesc.Usage = D3D11_USAGE_DEFAULT;							//// 사용모드 (DEFAULT는 그래픽카드에 생성)
			BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;				//// 어떤 버퍼로 사용되는지
			ZeroMemory(&SubresourceData, sizeof(D3D11_SUBRESOURCE_DATA));
			SubresourceData.pSysMem = vertices3D;
			if (FAILED(CreateBufferResult = g_pDevice->CreateBuffer(&BufferDesc, &SubresourceData, &m_pTargetBone->m_pVertexBuffer))) return false;
			//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

			//// 인덱스 버퍼 /////////////////////////////////////////////////////////////////////////////////////////////////////
			ZeroMemory(&BufferDesc, sizeof(D3D11_BUFFER_DESC));
			BufferDesc.ByteWidth = sizeof(DWORD) * m_pTargetBone->m_iIndexCount;	//// 사이즈 (Byte단위)
			BufferDesc.Usage = D3D11_USAGE_DEFAULT;							//// 사용모드 (DEFAULT는 그래픽카드에 생성)
			BufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;					//// 어떤 버퍼로 사용되는지
			ZeroMemory(&SubresourceData, sizeof(D3D11_SUBRESOURCE_DATA));
			SubresourceData.pSysMem = index3D;
			if (FAILED(CreateBufferResult = g_pDevice->CreateBuffer(&BufferDesc, &SubresourceData, &m_pTargetBone->m_pIndexBuffer))) return false;
			//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			iNumVertices = 0;
			iNumFaces = 0;
			vertices.clear();
			normals.clear();
			faces.clear();
			delete[] vertices3D;
			delete[] index3D;
		}
#pragma endregion
	}
	m_pTargetAni->m_TempSkeleton.Init();
	m_wordlist.clear();
	return true;
}