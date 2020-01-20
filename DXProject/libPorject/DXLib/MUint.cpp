#include "MUint.h"
#include "M3DObjectMgr.h"


bool MUnit::Create(M_STR name, M_STR sktname, M_STR* namelist, int namecount)
{
	m_pSkt = new MSkeleton;
	m_pSkt->m_name = name;
	MSkeleton* temp = I_SkeletonMgr[sktname];
	if (temp == nullptr) return false;
	m_pSkt->BindAni(temp->m_CurAni);
	m_name = name;
	for (int i = 0; i < namecount; i++)
	{
		int id = I_3DObjectMgr.GetInstanceModel(namelist[i])->CreateInstanceObject();
		M3DNObject* inst = I_3DObjectMgr.GetInstanceModel(namelist[i])->GetInstanceObject(id);
		m_ObjList.insert(make_pair((ITEM_TYPE)i, inst));
	}
	int jCount = temp->m_BoneList.size();
	for (int i = 0; i < jCount; i++)
	{
		M3DBone* oldbone = (*temp->m_BoneList.find(i)).second;
		M3DBone* newbone = m_pSkt->NewBone(oldbone->m_name);
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
				newbone->LinkParents(m_pSkt->Find(parent->m_name));
			}
		}
	}
	m_pSkt->Set();
	I_SkeletonMgr.Add(m_pSkt, m_name);
	return true;
}

bool MUnit::Change(ITEM_TYPE type, M_STR name)
{
	(*m_ObjList.find(type)).second->Release();
	delete (*m_ObjList.find(type)).second;
	int id = I_3DObjectMgr.GetInstanceModel(name)->CreateInstanceObject();
	(*m_ObjList.find(type)).second = I_3DObjectMgr.GetInstanceModel(name)->GetInstanceObject(id);
	return true;
}

bool MUnit::Frame()
{
	m_pSkt->Frame();
	m_Box.Updata(&m_WorldPosition, &m_WorldRotation, &m_WorldScale);
	return false;
}

bool MUnit::Render()
{
	m_pSkt->Render();
#if defined(DEBUG) || defined(_DEBUG)
	m_Box.Render();
#endif // DEBUG
	map<ITEM_TYPE, M3DNObject*>::iterator obj = m_ObjList.begin();
	for (map<ITEM_TYPE, M3DNObject*>::iterator obj = m_ObjList.begin(); obj != m_ObjList.end(); obj++)
	{
		(*obj).second->PreRender();
	}
	return false;
}

bool MUnit::Release()
{
#if defined(DEBUG) || defined(_DEBUG)
	m_Box.Release();
#endif // DEBUG
	map<ITEM_TYPE, M3DNObject*>::iterator obj = m_ObjList.begin();
	for (map<ITEM_TYPE, M3DNObject*>::iterator obj = m_ObjList.begin(); obj != m_ObjList.end(); obj++)
	{
		(*obj).second->Release();
		delete (*obj).second;
	}
	return true;
}

MUnit::MUnit()
{
}


MUnit::~MUnit()
{
}
