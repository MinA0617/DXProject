#include "MMesh.h"



MMesh::MMesh()
{
}


MMesh::~MMesh()
{
}

M_STR MMesh::GetName()
{
	return m_szName;
}


//float3 MMesh::GetTangetVector(float3 p0, float3 p1, float3 p2, float3 t0, float3 t1, float3 t2, float3 n1)
//{
//	D3DXVECTOR3 v0 = D3DXVECTOR3(p0.x, p0.y, p0.z);
//	D3DXVECTOR3 v1 = D3DXVECTOR3(p1.x, p1.y, p1.z);
//	D3DXVECTOR3 v2 = D3DXVECTOR3(p2.x, p2.y, p2.z);
//	D3DXVECTOR2 uv0 = D3DXVECTOR2(t0.x, t0.y);
//	D3DXVECTOR2 uv1 = D3DXVECTOR2(t1.x, t1.y);
//	D3DXVECTOR2 uv2 = D3DXVECTOR2(t2.x, t2.y);
//	D3DXVECTOR3 vNormal = D3DXVECTOR3(n1.x, n1.y, n1.z);
//
//	D3DXVECTOR3 vEdge1 = v1 - v0;
//	D3DXVECTOR3 vEdge2 = v2 - v0;
//	D3DXVec3Normalize(&vEdge1, &vEdge1);
//	D3DXVec3Normalize(&vEdge2, &vEdge2);
//	// UV delta
//	D3DXVECTOR2 deltaUV1 = uv1 - uv0;
//	D3DXVECTOR2 deltaUV2 = uv2 - uv0;
//	D3DXVec2Normalize(&deltaUV1, &deltaUV1);
//	D3DXVec2Normalize(&deltaUV2, &deltaUV2);
//
//	D3DXVECTOR3 vTangent;
//	float scale = (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);
//	float fDet = 1.0f / scale;
//	if (fabsf(fDet) < 1e-6f)
//	{
//		vTangent = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
//	}
//	else
//	{
//		vTangent = (vEdge1 * deltaUV2.y - vEdge2 * deltaUV1.y) * fDet;
//	}
//	D3DXVec3Normalize(&vTangent, &vTangent);
//	float3 result;
//	result.x = vTangent.x;
//	result.y = vTangent.y;
//	result.z = vTangent.z;
//	return result;
//}

//DWORD MMesh::Check(MVERTEX data)
//{
//	for (int i = 0; i < VertexData.size(); i++)
//	{
//		if (VertexData[i].p == data.p && VertexData[i].t == data.t && VertexData[i].n == data.n)
//		{
//			return i;
//		}
//	}
//	VertexData.push_back(data);
//	return VertexData.size() - 1;
//}

//bool MMesh::Load(M_STR filename)
//{
//#pragma region Count
//	int iMaxVertexCount = 0;
//	int iMaxUVCount = 0;
//	int iMaxNormalCount = 0;
//	int iMaxFaceCount = 0;
//
//	ifstream stream;
//	char input;
//	stream.open(filename);
//	if (stream.fail() == true)
//	{
//		return false;
//	}
//
//	stream.get(input); // ù���� ��������
//	while (!stream.eof()) // ���� ������ �� ����
//	{
//		switch (input)
//		{
//		case 'v':
//		{
//			stream.get(input);
//			// v�� ������ ��� �� ���� ���ڸ� �д´�.
//			switch (input)
//			{
//			case ' ':
//			{
//				iMaxVertexCount++;
//			}break;
//			case 't':
//			{
//				iMaxUVCount++;
//			}break;
//			case 'n':
//			{
//				iMaxNormalCount++;
//			}break;
//			default:
//				break;
//			}
//		}break;
//		case 'f':
//		{
//			stream.get(input);
//			if (input == ' ')
//			{
//				iMaxFaceCount++;
//			}
//		}break;
//		}
//		stream.get(input); // �ش���� ������ ������
//	}
//	stream.close();
//#pragma endregion Count
//
//#pragma region DataCreate
//	//// count��ŭ ���� �� �ʱ�ȭ ////
//	float3 *vertices = new float3[iMaxVertexCount];
//	float3 *texcoords = new float3[iMaxUVCount];
//	float3 *normals = new float3[iMaxNormalCount];
//	OBJIndex *faces = new OBJIndex[iMaxFaceCount * 3];
//
//	ZeroMemory(vertices, sizeof(M3Point) * iMaxVertexCount);
//	ZeroMemory(texcoords, sizeof(M3Point) * iMaxUVCount);
//	ZeroMemory(normals, sizeof(M3Point) * iMaxNormalCount);
//	ZeroMemory(faces, sizeof(OBJIndex) * iMaxFaceCount * 3);
//	///////////////////////////////////////////
//	//// Max�� �ƴ� ī��Ʈ�� ////
//	int iCurVertexCount = 0;
//	int	iCurUVCount = 0;
//	int	iCurNormalCount = 0;
//	int	iCurFaceCount = 0;
//	///////////////////////////////////////////
//
//	stream.open(filename);
//	//// ù������ ���ư��� ���Ͽ� �ٽ� ���� ////
//	stream.get(input); // ù���� ��������
//	while (!stream.eof()) // ���� ������ �� ����
//	{
//		switch (input)
//		{
//		case 'v':
//		{
//			stream.get(input);
//			// v�� ������ ��� �� ���� ���ڸ� �д´�.
//			switch (input)
//			{
//			case ' ':
//			{
//				stream >> vertices[iCurVertexCount].x >> vertices[iCurVertexCount].y >> vertices[iCurVertexCount].z;
//				iCurVertexCount++;
//			}break;
//			case 't':
//			{
//				stream >> texcoords[iCurUVCount].x >> texcoords[iCurUVCount].y >> texcoords[iCurUVCount].z;
//				texcoords[iCurUVCount].y = 1 - texcoords[iCurUVCount].y;
//				iCurUVCount++;
//			}break;
//			case 'n':
//			{
//				stream >> normals[iCurNormalCount].x >> normals[iCurNormalCount].y >> normals[iCurNormalCount].z;
//				iCurNormalCount++;
//			}break;
//			default:
//				break;
//			}
//		}break;
//		case 'f':
//		{
//			stream.get(input);
//			if (input == ' ')
//			{
//				char deletechar;
//				stream >> faces[iCurFaceCount].v >> deletechar >> faces[iCurFaceCount].vt >> deletechar >> faces[iCurFaceCount].vn;
//				iCurFaceCount++;
//				stream >> faces[iCurFaceCount].v >> deletechar >> faces[iCurFaceCount].vt >> deletechar >> faces[iCurFaceCount].vn;
//				iCurFaceCount++;
//				stream >> faces[iCurFaceCount].v >> deletechar >> faces[iCurFaceCount].vt >> deletechar >> faces[iCurFaceCount].vn;
//				iCurFaceCount++;
//			}
//		}
//		break;
//		}
//		while (input != '\n')
//		{
//			//// ������ ������� �о ������ ////
//			stream.get(input);
//		}
//		stream.get(input); // ��������
//	}
//	// �ε��� 1�� ���ֱ�
//	for (int i = 0; i < iMaxFaceCount * 3; i++)
//	{
//		faces[i].v = faces[i].v - 1;
//		faces[i].vt = faces[i].vt - 1;
//		faces[i].vn = faces[i].vn - 1;
//	}
//	stream.close();
//	//�ƿ�ǲ ������ ���� ������ֱ�? // �ϴ��� �����
//	//vector<P3VERTEX>	VertecData;
//	//vector<DWORD>		IndexData;
//	for (int i = 0; i < iMaxFaceCount * 3; i++)
//	{
//		MVERTEX newdata;
//		newdata.p = vertices[faces[i].v];
//		newdata.t = texcoords[faces[i].vt];
//		newdata.n = normals[faces[i].vn];
//		newdata.tv = GetTangetVector(vertices[faces[i].v], vertices[faces[i + 1].v], vertices[faces[i + 2].v], 
//			texcoords[faces[i].vt], texcoords[faces[i + 1].vt], texcoords[faces[i + 2].vt], normals[faces[i].vn]);
//		IndexData.push_back(Check(newdata));
//		i++;
//		newdata.p = vertices[faces[i].v];
//		newdata.t = texcoords[faces[i].vt];
//		newdata.n = normals[faces[i].vn];
//		newdata.tv = GetTangetVector(vertices[faces[i].v], vertices[faces[i + 1].v], vertices[faces[i - 1].v],
//			texcoords[faces[i].vt], texcoords[faces[i + 1].vt], texcoords[faces[i - 1].vt], normals[faces[i].vn]);
//		IndexData.push_back(Check(newdata));
//		i++;
//		newdata.p = vertices[faces[i].v];
//		newdata.t = texcoords[faces[i].vt];
//		newdata.n = normals[faces[i].vn];
//		newdata.tv = GetTangetVector(vertices[faces[i].v], vertices[faces[i - 2].v], vertices[faces[i - 1].v],
//			texcoords[faces[i].vt], texcoords[faces[i - 2].vt], texcoords[faces[i - 1].vt], normals[faces[i].vn]);
//		IndexData.push_back(Check(newdata));
//	}
//	m_iVertexCount = VertexData.size();
//	m_iIndexCount = IndexData.size();
//
//	delete[] vertices;
//	delete[] texcoords;
//	delete[] normals;
//	delete[] faces;
//#pragma endregion DataCreate
//	CreateBuffer();
//	return true;
//}

//bool MMesh::CreateBuffer()
//{
//	if (VertexData.size() == 0)
//	{
//		return false;
//	}
//	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	D3D11_BUFFER_DESC BufferDesc;				//// ���� ���� ����ũ
//	D3D11_SUBRESOURCE_DATA SubresourceData;		//// ���� ���ҽ� ������
//	HRESULT CreateBufferResult;					//// ���� �����
//	///// ���ؽ� ���� ///////////////////////////////////////////////////////////////////////////////////////////////////
//	ZeroMemory(&BufferDesc, sizeof(D3D11_BUFFER_DESC));
//	BufferDesc.ByteWidth = sizeof(MVERTEX) * VertexData.size();	//// ������ (Byte����)
//	BufferDesc.Usage = D3D11_USAGE_DEFAULT;							//// ����� (DEFAULT�� �׷���ī�忡 ����)
//	BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;				//// � ���۷� ���Ǵ���
//	ZeroMemory(&SubresourceData, sizeof(D3D11_SUBRESOURCE_DATA));
//	//SubresourceData.pSysMem = &VertecData[0];
//	SubresourceData.pSysMem = &VertexData.at(0);
//	if (FAILED(CreateBufferResult = g_pDevice->CreateBuffer(&BufferDesc, &SubresourceData, &m_pVertexBuffer))) return false;
//	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	//// �ε��� ���� /////////////////////////////////////////////////////////////////////////////////////////////////////
//	ZeroMemory(&BufferDesc, sizeof(D3D11_BUFFER_DESC));
//	BufferDesc.ByteWidth = sizeof(DWORD) * IndexData.size();	//// ������ (Byte����)
//	BufferDesc.Usage = D3D11_USAGE_DEFAULT;							//// ����� (DEFAULT�� �׷���ī�忡 ����)
//	BufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;					//// � ���۷� ���Ǵ���
//	ZeroMemory(&SubresourceData, sizeof(D3D11_SUBRESOURCE_DATA));
//	//SubresourceData.pSysMem = &IndexData[0];
//	SubresourceData.pSysMem = &IndexData.at(0);
//	if (FAILED(CreateBufferResult = g_pDevice->CreateBuffer(&BufferDesc, &SubresourceData, &m_pIndexBuffer))) return false;
//	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	return true;
//}


bool MMesh::Release()
{
	VertexData.clear();
	IndexData.clear();
	SAFE_RELEASE(m_pVertexBuffer);
	SAFE_RELEASE(m_pIndexBuffer);
	return true;
}