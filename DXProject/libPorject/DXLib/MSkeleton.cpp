#include "MSkeleton.h"



MSkeleton::MSkeleton()
{
	m_fAniSpeed = 1;
	iCount = 0;
	m_fMaxTime = 0;
	m_fAniTime = 0;
	m_fDelay = 0;
	m_bIsSlerp = false;
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

	if (m_ConstantZero.size() == 0)
		return false;
	BufferDesc.ByteWidth = sizeof(CONSTANT_BONE) * m_ConstantZero.size();

	ZeroMemory(&SubresourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	SubresourceData.pSysMem = &m_ConstantZero.at(0);
	if (FAILED(CreateBufferResult = g_pDevice->CreateBuffer(&BufferDesc, &SubresourceData, &m_pZeroBuffer))) return false;

	return true;
}

bool MSkeleton::BindAni(M_STR name, bool isCancel, float delay)
{
	// -----------------------------------------------------------------
	// 1. 캔슬하여 즉시 적용
	// 2. 캔슬하되 딜레이 적용
	// 3. 캔슬하지 않고 딜레이적용
	// 4. 딜레이상태의 경우 캔슬하고 즉시 적용
	// 5. 딜레이상태의 경우 캔슬하되 딜레이 적용
	// 6. 딜레이상태의 경우 캔슬하지 않고 딜레이 적용
	// 6. 다음애니가 입력애니와 같은경우 = 무시
	// 7. 딜레이상태의 경우 다음애니가 입력애니와 같은경우 = 무시
	// 8. 다음애니가 입력애니와 다른경우 = 딜레이상태로 전환
	// 9. 딜레이상태의 경우 다음애니가 입력애니와 다른경우 = 딜레이상태로 전환
	// -----------------------------------------------------------------
	if (m_bIsSlerp == false && m_CurAni == name) return false;
	MKeyAnimation* data = I_KeyAnimationMgr[name];
	if (data == nullptr) return false;
	if (isCancel)
	{
		if (delay == 0)
		{
			for (ITOR temp = m_BoneList.begin(); temp != m_BoneList.end(); temp++)
			{
				M3DBone* tempbone = (*temp).second;
				if (data->m_List.find((*temp).second->m_name) != data->m_List.end())
				{
					tempbone->m_KeyData = (*data->m_List.find((*temp).second->m_name)).second;
					tempbone->m_TempKeyData.m_vPositionKeyList.clear();
					tempbone->m_TempKeyData.m_vRotationKeyList.clear();
					tempbone->m_TempKeyData.m_vScaleKeyList.clear();
				}
			}
			m_fDelay = 0;
			m_bIsSlerp = false;
			m_CurAni = name;
			m_NextAni.clear();
			m_fAniTime = 0;
			m_fMaxTime = I_KeyAnimationMgr[m_CurAni]->GetMaxTime();
		}
		else
		{
			if (m_NextAni != name || m_bIsSlerp == false)
			{
				for (ITOR temp = m_BoneList.begin(); temp != m_BoneList.end(); temp++)
				{
					M3DBone* tempbone = (*temp).second;
					if (m_bIsSlerp)
					{
						M3DBone* tempbone = (*temp).second;
						tempbone->m_TempKeyData.m_vPositionKeyList.clear();
						tempbone->m_TempKeyData.m_vRotationKeyList.clear();
						tempbone->m_TempKeyData.m_vScaleKeyList.clear();
					}
					KEY_Position pos(0.0f, tempbone->GetLocalPosition());
					KEY_Rotation rot(0.0f, tempbone->GetLocalRotation());
					KEY_Scale scl(0.0f, tempbone->GetLocalScale());
					tempbone->m_TempKeyData.m_vPositionKeyList.push_back(pos);
					tempbone->m_TempKeyData.m_vRotationKeyList.push_back(rot);
					tempbone->m_TempKeyData.m_vScaleKeyList.push_back(scl);
					if (data->m_List.find(tempbone->m_name) != data->m_List.end())
					{
						KEY_Position pos2(delay, (*data->m_List.find(tempbone->m_name)).second->GetCurPosition(0.0f));
						KEY_Rotation rot2(delay, (*data->m_List.find(tempbone->m_name)).second->GetCurRotation(0.0f));
						KEY_Scale scl2(delay, (*data->m_List.find(tempbone->m_name)).second->GetCurScale(0.0f));
						tempbone->m_TempKeyData.m_vPositionKeyList.push_back(pos2);
						tempbone->m_TempKeyData.m_vRotationKeyList.push_back(rot2);
						tempbone->m_TempKeyData.m_vScaleKeyList.push_back(scl2);
					}
				}
				m_bIsSlerp = true;
				m_fDelay = 0;
				m_fAniTime = 0;
				m_fMaxTime = delay;
				m_NextAni = name;
			}
		}

	}
	else
	{
		m_NextAni = name;
		m_fDelay = delay;
		m_bIsSlerp = false;
	}
	return true;
}
M3DBone* MSkeleton::NewBone(M_STR name)
{
	if (m_pBoneBuffer != NULL)
	{
		return nullptr;
	}
	//ITOR findnode = m_BoneList.find(name);
	for (ITOR findnode = m_BoneList.begin(); findnode != m_BoneList.end(); findnode++)
	{
		if (name == (*findnode).second->m_name)
		{
			return (*findnode).second;
		}
	}
	M3DBone* data = new M3DBone;
	data->Init();
	data->m_name = name;
	m_BoneList.insert(std::make_pair(iCount++, data));
	return data;
}
M3DBone * MSkeleton::Find(M_STR name)
{
	for (ITOR findnode = m_BoneList.begin(); findnode != m_BoneList.end(); findnode++)
	{
		if (name == (*findnode).second->m_name)
		{
			return (*findnode).second;
		}
	}
	return nullptr;
}
void MSkeleton::SetZeroMat(M3DBone* data)
{
	M_STR name = data->m_name;
	if (m_ZeroMat.find(name) != m_ZeroMat.end())
	{
		data->ZeroMatrix = (*m_ZeroMat.find(name)).second;
	}
	else
	{
		data->SetZero();
	}
	return;
}
bool MSkeleton::AddZeroMat(M_STR name, D3DXVECTOR3 pos, D3DXQUATERNION rot, D3DXVECTOR3 scl)
{
	if (m_ZeroMat.find(name) == m_ZeroMat.end())
	{
		D3DXMATRIX temp;
		D3DXMatrixTransformation(&temp, NULL, NULL, &scl, NULL, &rot, &pos);
		D3DXMatrixInverse(&temp, NULL, &temp);
		D3DXMatrixTranspose(&temp, &temp);
		m_ZeroMat.insert(make_pair(name, temp));
	}
	return false;
}
int MSkeleton::NumofBone(M_STR name)
{
	for (ITOR data = m_BoneList.begin(); data != m_BoneList.end(); data++)
	{
		if ((*data).second->m_name == name)
		{
			return (*data).first;
		}
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
	while (m_fAniTime > m_fMaxTime)
	{
		if (m_NextAni.size())
		{
			if (m_fDelay == 0)
			{
				BindAni(m_NextAni);
				m_fAniTime = 0;
				break;
			}
			else
			{
				BindAni(m_NextAni, true, m_fDelay);
				m_fAniTime = 0;
				break;
			}
		}
		if (m_fMaxTime == 0)
		{
			break;
		}
		m_fAniTime -= m_fMaxTime;
	}

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
	for (auto data : m_pChildList)
	{
		data->Render();
	}
#endif
	return true;
}

bool MSkeleton::Release()
{
	for (ITOR data = m_BoneList.begin(); data != m_BoneList.end(); data++)
	{
		(*data).second->Release();
		delete (*data).second;
	}
	//for (auto data : m_pChildList)
	//{
	//	data->Release();
	//}
	m_BoneList.clear();
	SAFE_RELEASE(m_pBoneBuffer);
	SAFE_RELEASE(m_pZeroBuffer);
	return true;
}

bool MSkeleton::Set()
{
	PreFrame();
	for (ITOR data = m_BoneList.begin(); data != m_BoneList.end(); data++)
	{
		M3DBone* tempbone = (*data).second;
		// Set Root
		if (tempbone->IsHasParents() == false)
		{
			tempbone->LinkParents(this);
		}
		// Set Zero
		//tempbone->SetZero();
		// Set Constant
		CONSTANT_BONE temp;
		temp.matBoneWorld = tempbone->GetConstantOBJ().matWorld;
		m_ConstantBone.push_back(temp);
		SetZeroMat(tempbone);
		temp.matBoneWorld = tempbone->ZeroMatrix;
		m_ConstantZero.push_back(temp);

		// Set Maxtime
		m_fMaxTime = I_KeyAnimationMgr[m_CurAni]->GetMaxTime();
	}
	// Create Buffer
	CreateBoneBuffer();
	// Clear ZeroMatData
	m_ZeroMat.clear();
	return true;
}