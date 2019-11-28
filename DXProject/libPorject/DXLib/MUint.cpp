#include "MUint.h"
#include "M3DObjectMgr.h"


bool MUnit::Create(M_STR name, M_STR sktname, M_STR* namelist, int namecount)
{
	skt = new MSkeleton;
	skt->m_name = name;
	MSkeleton* temp = I_SkeletonMgr[sktname];
	if (temp == nullptr) return false;
	skt->BindAni(temp->m_CurAni);
	m_name = name;
	m_iCount = namecount;
	for (int i = 0; i < namecount; i++)
	{
		obj[i] = new MSkinModel;
		obj[i]->Copy(I_3DObjectMgr[namelist[i]]);
		obj[i]->m_Skeleton = skt;
	}
	int jCount = temp->m_BoneList.size();
	for (int i = 0; i < jCount; i++)
	{
		M3DBone* oldbone = (*temp->m_BoneList.find(i)).second;
		M3DBone* newbone = skt->NewBone(oldbone->m_name);
		if (newbone != nullptr)
		{
			if (oldbone->m_Box)
			{
				MBoundingBox* box = new MBoundingBox;
#if defined(DEBUG) || defined(_DEBUG)
				box->Init();
#endif // DEBUG
				box->Copy(oldbone->m_Box);
				newbone->m_Box = box;
			}
			newbone->m_name = oldbone->m_name;
			newbone->SetLocalPosition(oldbone->GetLocalPosition());
			newbone->SetLocalRotation(oldbone->GetLocalRotation());
			newbone->SetLocalScale(oldbone->GetLocalScale());
			newbone->CreateConstantBuffer();
			newbone->m_KeyData = oldbone->m_KeyData;
			M3DObject* parent = oldbone->GetParents();
			if (parent != temp)
			{
				newbone->LinkParents(skt->Find(parent->m_name));
			}
		}
	}
	skt->Set();
	I_SkeletonMgr.Add(skt, m_name);
	return true;
}

bool MUnit::Change(ITEM_TYPE type, M_STR name)
{
	obj[type]->Release();
	delete obj[type];
	obj[type] = new MSkinModel;
	obj[type]->Copy(I_3DObjectMgr[name]);
	obj[type]->m_Skeleton = skt;
	return true;
}

void MUnit::UpdateBox()
{
}

bool MUnit::Init()
{
	return true;
}

bool MUnit::Frame()
{
	skt->Frame();
	if (m_Box)m_Box->Updata(&m_WorldPosition, &m_WorldRotation, &m_WorldScale);
	for (int i = 0; i < m_iCount; i++)
	{
		obj[i]->Frame();
	}
	return false;
}

bool MUnit::Render()
{
	skt->Render();
#if defined(DEBUG) || defined(_DEBUG)
	if (m_Box)m_Box->Render();
#endif // DEBUG
	for (int i = 0; i < m_iCount; i++)
	{
		obj[i]->Render();
	}
	return false;
}

bool MUnit::Release()
{
#if defined(DEBUG) || defined(_DEBUG)
	SAFE_RELEASE(m_Box);
#endif // DEBUG
	SAFE_DELETE(m_Box);
	for (int i = 0; i < m_iCount; i++)
	{
		obj[i]->Release();
		delete obj[i];
	}
	return false;
}

MUnit::MUnit()
{
	m_iCount = 0;
	for (int i = 0; i < MAX_OBJ; i++)
	{
		obj[i] = nullptr;
	}
}


MUnit::~MUnit()
{
}
