#include "MParser2.h"



MParser2::MParser2()
{
}


MParser2::~MParser2()
{
}

bool MParser2::LoadKeyAnimation(M_STR filename)
{
	ifstream stream;
	stream.open(filename);
	if (stream.fail() == true)
	{
		return false;
	}
	string word;
	typedef string::iterator ITER;
	while (!stream.eof())
	{
		getline(stream, word);
		ITER start = word.begin();
		for (ITER data = word.begin(); data != word.end(); data++)
		{
			if (*data == '\t' || *data == ' ' || *data == '"')
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
	_tsplitpath_s(filename.c_str(), Drive, Dir, Name, Ext); // �н��� �̸��� Ȯ���� �����ֱ�
	M_STR name = Name; // �̸�����
	name += Ext; // Ȯ���� �߰��ؼ� ����
	return CreateData(name);
}

bool MParser2::CreateData(M_STR name)
{
	typedef vector<string>::iterator ITER;
	if (m_wordlist.size() == 0)
	{
		return false;
	}

	MKeyAnimation*	m_pTargetAni = nullptr;		// �����͸� ���� �� Ÿ��
	M3DBone*		m_pTargetBone = nullptr;		// �����͸� ���� �� Ÿ��


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
			// �ִϸ��̼� �̸��� ������ �ִϸ��̼� ����
			data++;
			m_pTargetAni = I_KeyAnimationMgr.NewKeyAnimation(name);
		}
		if (*data == "*SCENE_FIRSTFRAME")
		{
			// ���� ������
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
			// ������ ������
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
			// ������ �� ������
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
			// ƽ �� ������
		}
		if (*data == "*NODE_NAME")
		{
			// ��� ������ ������ ���� �͵��� ���� ����� �ٲ�. �ߺ�üũ�Ͽ� ���ο� ����� �߰���Ų�� 
			if (m_pTargetAni != nullptr)
			{
				data++;
				string temp = *data;
				M_STR name;
				name.append(temp.begin(), temp.end());
				m_pTargetBone = m_pTargetAni->m_TempSkeleton.NewBone(name);
				// �����̼�Ű ���ʰ� �ֱ�
			}

		}
		if (*data == "*TM_POS")
		{
			// �������
			if (m_pTargetAni != nullptr)
			{
				data++;
				string temp = *data;
				float x = atof(temp.c_str());
				data++;
				temp = *data;
				float z = atof(temp.c_str());
				data++;
				temp = *data;
				float y = atof(temp.c_str());
				m_pTargetBone->SetLocalPosition(D3DXVECTOR3(x, y, z));
			}
		}
		if (*data == "*TM_ROTAXIS")
		{
			// �������
			if (m_pTargetAni != nullptr)
			{
				data++;
				string temp = *data;
				float y = atof(temp.c_str());
				data++;
				temp = *data;
				float x = atof(temp.c_str());
				data++;
				temp = *data;
				float z = atof(temp.c_str());
				data += 2;
				temp = *data;
				float w = atof(temp.c_str());
				D3DXQUATERNION qRotation;
				D3DXQuaternionRotationAxis(&qRotation, &D3DXVECTOR3(x, y, z), w);
				//D3DXQuaternionRotationAxis(&qRotation, &D3DXVECTOR3(1, 0, 0), 0);
				m_pTargetBone->SetLocalRotation(qRotation);
			}
		}
		if (*data == "*TM_SCALE")
		{
			// �������
			if (m_pTargetAni != nullptr)
			{
				//m_pTargetBone->m_ZeroScale = D3DXVECTOR3(1, 1, 1);
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
				float z = atof(temp.c_str());
				data++;
				temp = *data;
				float x = atof(temp.c_str());
				data++;
				temp = *data;
				float y = atof(temp.c_str());
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
				float y = -atof(temp.c_str());
				data++;
				temp = *data;
				float x = atof(temp.c_str());
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
				float z = atof(temp.c_str());
				data++;
				temp = *data;
				float y = atof(temp.c_str());
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
				// ���ؽ�
				D3DXVECTOR3 pos = m_pTargetBone->GetPosition();
				data ++;
				string temp = *data;
				int index = atoi(temp.c_str());
				data++;
				temp = *data;
				float x = atof(temp.c_str()) - pos.x;
				data++;
				temp = *data;
				float z = atof(temp.c_str()) - pos.z;
				data++;
				temp = *data;
				float y = atof(temp.c_str()) - pos.y;
				//if (m_vertices.find(index) == m_vertices.end())
				vertices.insert(std::make_pair(index, float3(x, y, z)));
			}
		}
		if (*data == "*MESH_VERTEXNORMAL")
		{
			if (m_pTargetAni != nullptr)
			{
				// �븻
				data++;
				string temp = *data;
				int index = atoi(temp.c_str());
				data++;
				temp = *data;
				float x = atof(temp.c_str());
				data++;
				temp = *data;
				float z = atof(temp.c_str());
				data++;
				temp = *data;
				float y = atof(temp.c_str());
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
				// ���̽�
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
				data++;
				string temp = *data;
				M_STR name;
				name.append(temp.begin(), temp.end());
				M3DObject* parents = (*(m_pTargetAni->m_TempSkeleton.m_BoneList.find(name))).second;
				m_pTargetBone->LinkParents(parents);
			}
		}
		//if (*data == "*MESH_TVERT")
		//{
		//	if (m_pTargetAni != nullptr)
		//	{
		//		// UV���ؽ�
		//		data++;
		//		string temp = *data;
		//		int index = atoi(temp.c_str());
		//		data++;
		//		float x = atof(temp.c_str());
		//		data++;
		//		temp = *data;
		//		float y = atof(temp.c_str());
		//		data++;
		//		temp = *data;
		//		float z = atof(temp.c_str());
		//		if (m_texcoords.find(index) == m_texcoords.end())
		//		{
		//			m_texcoords.insert(std::make_pair(index, float3(x, y, z)));
		//		}
		//	}
		//}
#pragma region CreateBuffer
		if (iNumVertices == vertices.size() && 
			iNumVertices == normals.size() && 
			iNumFaces == faces.size() &&
			iNumVertices != 0 &&
			iNumFaces != 0)
		{
			// ���ڿ� ������� Obj����
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
			D3D11_BUFFER_DESC BufferDesc;				//// ���� ���� ����ũ
			D3D11_SUBRESOURCE_DATA SubresourceData;		//// ���� ���ҽ� ������
			HRESULT CreateBufferResult;					//// ���� �����
			///// ���ؽ� ���� ///////////////////////////////////////////////////////////////////////////////////////////////////
			ZeroMemory(&BufferDesc, sizeof(D3D11_BUFFER_DESC));
			BufferDesc.ByteWidth = sizeof(P3VERTEX) * m_pTargetBone->m_iVertexCount;	//// ������ (Byte����)
			BufferDesc.Usage = D3D11_USAGE_DEFAULT;							//// ����� (DEFAULT�� �׷���ī�忡 ����)
			BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;				//// � ���۷� ���Ǵ���
			ZeroMemory(&SubresourceData, sizeof(D3D11_SUBRESOURCE_DATA));
			SubresourceData.pSysMem = vertices3D;
			if (FAILED(CreateBufferResult = g_pDevice->CreateBuffer(&BufferDesc, &SubresourceData, &m_pTargetBone->m_pVertexBuffer))) return false;
			//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

			//// �ε��� ���� /////////////////////////////////////////////////////////////////////////////////////////////////////
			ZeroMemory(&BufferDesc, sizeof(D3D11_BUFFER_DESC));
			BufferDesc.ByteWidth = sizeof(DWORD) * m_pTargetBone->m_iIndexCount;	//// ������ (Byte����)
			BufferDesc.Usage = D3D11_USAGE_DEFAULT;							//// ����� (DEFAULT�� �׷���ī�忡 ����)
			BufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;					//// � ���۷� ���Ǵ���
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

