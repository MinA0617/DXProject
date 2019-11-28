#include "M3DInstanceModel.h"
#include "MCameraMgr.h"
#include "M3DObjectMgr.h"

bool M3DInstanceModel::CreateInstanceBuffer()
{
	if (m_iSize == 0) return false;
	SAFE_RELEASE(m_InstanceBuffer);
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	D3D11_BUFFER_DESC BufferDesc;				//// 공용 버퍼 데스크
	D3D11_SUBRESOURCE_DATA SubresourceData;		//// 공용 리소스 데이터
	HRESULT CreateBufferResult;					//// 공용 결과값
	///// 버텍스 버퍼 ///////////////////////////////////////////////////////////////////////////////////////////////////
	ZeroMemory(&BufferDesc, sizeof(D3D11_BUFFER_DESC));
	BufferDesc.ByteWidth = sizeof(D3DXMATRIX) * m_iSize;		//// 사이즈 (Byte단위)
	BufferDesc.Usage = D3D11_USAGE_DYNAMIC;							//// 사용모드 (DEFAULT는 그래픽카드에 생성)
	BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;				//// 어떤 버퍼로 사용되는지
	BufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	if (FAILED(CreateBufferResult = g_pDevice->CreateBuffer(&BufferDesc, NULL, &m_InstanceBuffer))) return false;
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	return true;
}

bool M3DInstanceModel::Load_MAP(M_STR filename, MAPTYPE type)
{
	return m_Material->Load(filename, type);
}

int M3DInstanceModel::CreateInstanceObject(int count)
{
	if (count <= 0) return -1;
	int flag = m_iCount;
	for (int i = 0; i < count; i++)
	{
		M3DInstance* data = new M3DInstance(this);
		data->Init();
		data->m_Box.Copy(&m_Box);
		m_InstanceList.insert(make_pair(m_iCount, data));
		m_iCount++;
		m_iSize++;
	}
	CreateInstanceBuffer();
	return flag;
}

M3DInstance * M3DInstanceModel::GetInstanceObject(int index)
{
	map<int,M3DInstance*>::iterator temp = m_InstanceList.find(index);
	if (temp != m_InstanceList.end())
	{
		return (*temp).second;
	}
	return nullptr;
}

bool M3DInstanceModel::DeleteInstanceObject(int index)
{
	map<int, M3DInstance*>::iterator temp = m_InstanceList.find(index);
	if (temp != m_InstanceList.end())
	{
		I_3DObjectMgr.m_pTree->DeleteInstancObject((*temp).second);
		(*temp).second->Release();
		delete (*temp).second;
		m_InstanceList.erase(index);
		m_iSize--;
		CreateInstanceBuffer();
		return true;
	}
	return false;
}

bool M3DInstanceModel::AddRenderObject(M3DInstance * obj)
{
	// 노드만 확인해서 오기 때문에 프로스텀도 돌린다.
	if (I_CameraMgr.frustum.CheckOBB(&obj->m_Box))
	{
		m_MatrixList.push_back(obj->m_WorldMatrix);
		return true;
	}
	return false;
}

bool M3DInstanceModel::Init()
{
	int index = I_MaterialMgr.CreateNewMaterial();
	m_Material = I_MaterialMgr[index];
	m_Material->m_PixelShaderID = PS3DINSTANCE;
	return true;
}

bool M3DInstanceModel::Frame()
{
	if (m_InstanceBuffer == NULL) return false;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	HRESULT result = g_pImmediateContext->Map((ID3D11Resource*)m_InstanceBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (!FAILED(result))
	{
		D3DXMATRIX*	pmat = (D3DXMATRIX*)mappedResource.pData;
		for (int i = 0; i < m_MatrixList.size(); i++)
		{
			pmat[i] = m_MatrixList[i];
		}
		g_pImmediateContext->Unmap(m_InstanceBuffer, 0);
	}
	return true;
}

bool M3DInstanceModel::Render()
{
	///// 스테이트 매니져로 블랜드셋팅을 받아오기 ////
	m_Material->Render();

	g_pImmediateContext->IASetInputLayout(I_VertexShaderMgr.m_LOList[m_Mesh->m_VertexShaderID]);
	g_pImmediateContext->VSSetShader(I_VertexShaderMgr.m_VSList[m_Mesh->m_VertexShaderID], NULL, 0);

	UINT stride = sizeof(MVERTEX);	UINT offset = 0;
	g_pImmediateContext->IASetVertexBuffers(0, 1, &m_Mesh->m_pVertexBuffer, &stride, &offset);

	stride = sizeof(D3DXMATRIX);
	g_pImmediateContext->IASetVertexBuffers(1, 1, &m_InstanceBuffer, &stride, &offset);

	g_pImmediateContext->IASetIndexBuffer(m_Mesh->m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	g_pImmediateContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	g_pImmediateContext->DrawIndexedInstanced(m_Mesh->m_iIndexCount, m_MatrixList.size(), 0, 0, 0);
	m_MatrixList.clear();
#if defined(DEBUG) || defined(_DEBUG)
	if (g_isBoxRender)
	{
		for (auto temp : m_InstanceList)
		{
			temp.second->m_Box.Render();
		}
	}
#endif // DEBUG
	return true;
}

bool M3DInstanceModel::Release()
{
	for (auto temp : m_InstanceList)
	{
		temp.second->Release();
		delete temp.second;
	}
	SAFE_RELEASE(m_InstanceBuffer);
	return true;
}

M3DInstanceModel::M3DInstanceModel()
{
	m_Mesh = nullptr;
	m_Material = nullptr;
	m_iCount = 0;
	m_iSize = 0;
}


M3DInstanceModel::~M3DInstanceModel()
{
}
