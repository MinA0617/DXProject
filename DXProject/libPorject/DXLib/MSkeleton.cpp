#include "MSkeleton.h"



MSkeleton::MSkeleton()
{
	m_fAniSpeed = 1;
}


MSkeleton::~MSkeleton()
{
}

bool MSkeleton::CreateBoneBuffer()
{
	if (m_ConstantBone.size() == 0)
		return false;
	D3D11_BUFFER_DESC BufferDesc;
	HRESULT CreateBufferResult;
	D3D11_SUBRESOURCE_DATA SubresourceData;

	ZeroMemory(&BufferDesc, sizeof(D3D11_BUFFER_DESC));
	BufferDesc.ByteWidth = sizeof(CONSTANT_BONE) * m_ConstantBone.size();
	BufferDesc.Usage = D3D11_USAGE_DEFAULT;
	BufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

	ZeroMemory(&SubresourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	SubresourceData.pSysMem = &m_ConstantBone.at(0);

	if (FAILED(CreateBufferResult = g_pDevice->CreateBuffer(&BufferDesc, &SubresourceData, &m_pBoneBuffer))) return false;
	return true;
}


M3DBone* MSkeleton::NewBone(M_STR name)
{
	if (m_pBoneBuffer != NULL)
	{
		return nullptr;
	}
	ITOR findnode = m_BoneList.find(name);
	if (findnode == m_BoneList.end())
	{
		M3DBone* data = new M3DBone;
		data->Init();
		m_BoneList.insert(std::make_pair(name, data));
		return data;
	}
	return (*findnode).second;
}
bool MSkeleton::BindAni(MKeyAnimation* data)
{
	for (ITOR temp = m_BoneList.begin(); temp != m_BoneList.end(); temp++)
	{
		(*temp).second->m_KeyData = (*data->m_List.find((*temp).first)).second;
	}
	return true;
}
int MSkeleton::NumofBone(M_STR name)
{
	int iNum = 0;
	for (ITOR data = m_BoneList.begin(); data != m_BoneList.end(); data++)
	{
		if ((*data).first == name)
		{
			return iNum;
		}
		iNum++;
	}
	return -1;
}
bool MSkeleton::Init()
{
	m_fAniTime = 0;
#if defined(DEBUG) || defined(_DEBUG)
	CreateConstantBuffer();
#endif
	for (ITOR data = m_BoneList.begin(); data != m_BoneList.end(); data++)
	{
		if ((*data).second->IsHasParents() == false)
		{
			(*data).second->LinkParents(this);
		}
	}
	return true;
}

bool MSkeleton::Frame()
{
	m_fAniTime += g_fSeoundPerFrame * m_fAniSpeed;
	int i = 0;
	for (ITOR data = m_BoneList.begin(); data != m_BoneList.end(); data++)
	{
		(*data).second->UpdateKey(m_fAniTime);
		(*data).second->Frame();
		m_ConstantBone[i].matBoneWorld = (*data).second->m_ConstantOBJ.matWorld;
		i++;
	}
	PreFrame();
	g_pImmediateContext->UpdateSubresource(m_pBoneBuffer, 0, 0, &m_ConstantBone.at(0), 0, 0);
	return true;
}

bool MSkeleton::Render()
{
#if defined(DEBUG) || defined(_DEBUG)
	for (ITOR data = m_BoneList.begin(); data != m_BoneList.end(); data++)
	{
		(*data).second->Render();
	}
#endif
	return true;
}

bool MSkeleton::Release()
{
	for (ITOR data = m_BoneList.begin(); data != m_BoneList.end(); data++)
	{
		(*data).second->Release();
	}
	m_BoneList.clear();
	SAFE_RELEASE(m_pBoneBuffer);
	return true;
}

bool MSkeleton::Set()
{
	for (ITOR data = m_BoneList.begin(); data != m_BoneList.end(); data++)
	{
		// Set Zero
		(*data).second->SetZero();
		// Set Root
		if ((*data).second->IsHasParents() == false)
		{
			(*data).second->LinkParents(this);
		}
		// Set Constant
		CONSTANT_BONE temp;
		temp.matBoneWorld = (*data).second->GetConstantOBJ().matWorld;
		m_ConstantBone.push_back(temp);
	}
	// Create Buffer
	CreateBoneBuffer();
	return true;
}