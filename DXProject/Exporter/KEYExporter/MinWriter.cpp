#include "stdafx.h"
#include "MinWriter.h"

void MOBJWriter::Set(const TCHAR * name, Interface * pMax)
{
	m_pMax = pMax;
	m_filename = name;
	m_pRootNode = m_pMax->GetRootNode(); // 맥스에선 모든 객체가 트리화 되어있어서 루트노드를 찾아둔다.
	PreProcess(m_pRootNode);
}

void MOBJWriter::PreProcess(INode * pNode)
{
	if (pNode == NULL) return;
	AddObject(pNode);
	int iNumChildren = pNode->NumberOfChildren();
	for (int i = 0; i < iNumChildren; i++)
	{
		INode* child = pNode->GetChildNode(i);
		PreProcess(child);
	}
}

void MOBJWriter::AddObject(INode * pNode)
{
	//GEOMOBJECT_CLASS_ID;
	//HELPER_CLASS_ID;
	
	ObjectState os = pNode->EvalWorldState(0);
	if (os.obj)
	{
		switch (os.obj->SuperClassID())
		{
		case GEOMOBJECT_CLASS_ID:
			m_ObjectList.push_back(pNode);
			break;
		case HELPER_CLASS_ID:
			m_ObjectList.push_back(pNode);
			break;
		default:
			break;
		}
	}
}

int MOBJWriter::SearchType(INode * pNode)
{
	ObjectState os = pNode->EvalWorldState(0);
	if (os.obj)
	{
		switch (os.obj->SuperClassID())
		{
		case GEOMOBJECT_CLASS_ID:
		{
			if (os.obj->ClassID() == BONE_OBJ_CLASSID)
			{
				return 1;
				break;
			}
			else
			{
				return 0;
			}
		}
		case HELPER_CLASS_ID:
			return 1;
			break;
		default:
			break;
		}
	}
	return 0;
}

void MOBJWriter::ExportAnimationKeys(INode * pNode)
{
	// 스케일은 나중에 추가
	float step = 800;
	_ftprintf(m_pStream, L"ANIMATION_KEY\n");
	Control* pC = pNode->GetTMController()->GetPositionController();
	Control* rC = pNode->GetTMController()->GetRotationController();
	Control* sC = pNode->GetTMController()->GetScaleController();
	int iNumPos = pC->NumKeys();
	int iNumRot = rC->NumKeys();
	int iNumScl = sC->NumKeys();
	float TickPerSecond = 4800.00f;
	TimeValue start = m_pMax->GetAnimRange().Start();
	TimeValue end = m_pMax->GetAnimRange().End();
	vector<Point3>	poslist;
	vector<int>		postime;
	vector<Quat>	rotlist;
	vector<int>		rottime;
	vector<Point3>	scllist;
	vector<int>		scltime;
	{
		// ------------ 스타트 키 추가 ------------
		Matrix3 CurTm = pNode->GetNodeTM(start)  * Inverse(pNode->GetParentTM(start));
		AffineParts ap;
		decomp_affine(CurTm, &ap);
		// ----------------------------------------
		Point3 pos = ap.t;
		SwapFloat(pos.y, pos.z);
		poslist.push_back(pos);
		postime.push_back(start);
		// ----------------------------------------
		AngAxis temprot(CurTm);
		SwapFloat(temprot.axis.y, temprot.axis.z);
		Quat rot(temprot);
		rotlist.push_back(rot);
		rottime.push_back(start);
		// ----------------------------------------
		Point3 scl = ap.k;
		SwapFloat(scl.y, scl.z);
		scllist.push_back(scl);
		scltime.push_back(start);

	}
	// ------------ 키 정보 추가 ------------
	for (int i = start + step;; i = i + step)
	{
		if (i >= end)
		{
			Matrix3 CurTm = pNode->GetNodeTM(end) * Inverse(pNode->GetParentTM(end));
			AffineParts ap;
			decomp_affine(CurTm, &ap);
			if (iNumPos > 1)
			{
				Point3 pos = ap.t;
				SwapFloat(pos.y, pos.z);
				if (!poslist[poslist.size() - 1].Equals(pos))
				{
					poslist.push_back(pos);
					postime.push_back(end);
				}
			}
			if (iNumRot > 1)
			{
				AngAxis temprot(CurTm);
				SwapFloat(temprot.axis.y, temprot.axis.z);
				Quat rot(temprot);
				if (!rotlist[rotlist.size() - 1].Equals(rot))
				{
					rotlist.push_back(rot);
					rottime.push_back(end);
				}
			}
			if (iNumScl > 1)
			{
				Point3 scl = ap.k;
				SwapFloat(scl.y, scl.z);
				if (!scllist[scllist.size() - 1].Equals(scl))
				{
					scllist.push_back(scl);
					scltime.push_back(end);
				}
			}
			break;
		}
		Matrix3 CurTm = pNode->GetNodeTM(i) * Inverse(pNode->GetParentTM(i));
		AffineParts ap;
		decomp_affine(CurTm, &ap);
		if (iNumPos > 1)
		{
			Point3 pos = ap.t;
			SwapFloat(pos.y, pos.z);
			if (!poslist[poslist.size() - 1].Equals(pos))
			{
				poslist.push_back(pos);
				postime.push_back(i);
			}
		}
		if (iNumRot > 1)
		{
			AngAxis temprot(CurTm);
			SwapFloat(temprot.axis.y, temprot.axis.z);
			Quat rot(temprot);
			if (!rotlist[rotlist.size() - 1].Equals(rot))
			{
				rotlist.push_back(rot);
				rottime.push_back(i);
			}
		}
		if (iNumScl > 1)
		{
			Point3 scl = ap.k;
			SwapFloat(scl.y, scl.z);
			if (!scllist[scllist.size() - 1].Equals(scl))
			{
				scllist.push_back(scl);
				scltime.push_back(i);
			}
		}
	}
	// ------------ 정보 추출 ------------
	iNumPos = poslist.size();
	iNumRot = rotlist.size();
	iNumScl = scllist.size();
	_ftprintf(m_pStream, L"POSITION_KEY_COUNT %d\n", iNumPos);
	for (int i = 0; i < iNumPos; i++)
	{
		_ftprintf(m_pStream, L"%10.4f ", postime[i] / TickPerSecond);
		_ftprintf(m_pStream, L"%10.4f %10.4f %10.4f\n", poslist[i].x, poslist[i].y, poslist[i].z);
	}
	_ftprintf(m_pStream, L"ROTATION_KEY_COUNT %d\n", iNumRot);
	for (int i = 0; i < iNumRot; i++)
	{
		_ftprintf(m_pStream, L"%10.4f ", rottime[i] / TickPerSecond);
		_ftprintf(m_pStream, L"%10.4f %10.4f %10.4f %10.4f\n", rotlist[i].x, rotlist[i].y, rotlist[i].z, rotlist[i].w);
	}
	_ftprintf(m_pStream, L"SCALE_KEY_COUNT %d\n", iNumScl);
	for (int i = 0; i < iNumScl; i++)
	{
		_ftprintf(m_pStream, L"%10.4f ", scltime[i] / TickPerSecond);
		_ftprintf(m_pStream, L"%10.4f %10.4f %10.4f\n", scllist[i].x, scllist[i].y, scllist[i].z);
	}
	_ftprintf(m_pStream, L"ANIMATION_END\n");
}

void MOBJWriter::SwapFloat(float & data1, float & data2)
{
	float temp = data1;
	data1 = data2;
	data2 = temp;
}

bool MOBJWriter::Export()
{
	_wfopen_s(&m_pStream, m_filename.c_str(), L"w");
	// -------------------------------------------------------------------------------
	for (int i = 0; i < m_ObjectList.size(); i++)
	{
		if(SearchType(m_ObjectList[i]))
		{
			_ftprintf(m_pStream, L"TYPE KEYDATA\n");
		}
		_ftprintf(m_pStream, L"OBJECT_NAME ");
		//_ftprintf(m_pStream, L"%s\n", m_ObjectList[i]->GetName());
		ExportName(m_ObjectList[i]->GetName());
		_ftprintf(m_pStream, L"\n");
		ExportAnimationKeys(m_ObjectList[i]);
		_ftprintf(m_pStream, L"OBJECT_END\n");
	}
	// -------------------------------------------------------------------------------
	fclose(m_pStream);
	MessageBox(GetActiveWindow(), m_filename.c_str(), _T("성공!"), MB_OK); // 확인 메세지 상자
	return true;
}

void MOBJWriter::ExportName(const MCHAR* name)
{
	//MCHAR* name = const_cast<MCHAR*>(pNode->GetName());
	wstring buffer;
	buffer.append(name);
	for (int i = 0; i < buffer.length(); i++)
	{
		if (buffer[i] == 32)
		{
			_ftprintf(m_pStream, L"_");
		}
		else
		{
			_ftprintf(m_pStream, L"%c", buffer[i]);
		}
	}
}

MOBJWriter::MOBJWriter()
{
}


MOBJWriter::~MOBJWriter()
{
}
