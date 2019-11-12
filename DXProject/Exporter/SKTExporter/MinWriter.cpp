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

UINT MOBJWriter::CheckMVERTEX(vector<MVERTEX> &list, MVERTEX data)
{
	for (int i = 0; i < list.size(); i++)
	{
		if (list[i].p == data.p && list[i].t == data.t && list[i].n == data.n)
		{
			return i;
		}
	}
	list.push_back(data);
	return list.size() - 1;
}

UINT MOBJWriter::CheckCVERTEX(vector<CVERTEX> &list, CVERTEX data)
{
	for (int i = 0; i < list.size(); i++)
	{
		if (list[i].p == data.p && list[i].t == data.t && list[i].n == data.n)
		{
			return i;
		}
	}
	list.push_back(data);
	return list.size() - 1;
}

TriObject* MOBJWriter::GetTriObjectFromNode(INode * pNode, TimeValue tick, bool& deleteit)
{
	Object* obj = pNode->EvalWorldState(tick).obj; // 해당 시간의 상태의 오브젝트를 가져옴
	if (obj->CanConvertToType(Class_ID(TRIOBJ_CLASS_ID, 0))) // 트라이앵글 오브젝트로 변환이 가능한지 여부
	{
		TriObject* tri = (TriObject*)obj->ConvertToType(tick, Class_ID(TRIOBJ_CLASS_ID, 0));
		if (obj != tri)
		{
			deleteit = true;
		}
		return tri;
	}
	return nullptr;
}

void MOBJWriter::ExportParent(INode * pNode)
{
	INode* parent = pNode->GetParentNode();
	if (parent->IsRootNode() == false)
	{
		//_ftprintf(m_pStream, L"PARENT %s\n", parent->GetName());
		_ftprintf(m_pStream, L"PARENT ");
		ExportName(parent->GetName());
		_ftprintf(m_pStream, L"\n");
	}
	else
	{
		_ftprintf(m_pStream, L"PARENT NULL\n");
	}
}

void MOBJWriter::ExportMatrix(INode * pNode)
{
	Matrix3 pivot = pNode->GetNodeTM(0) * Inverse(pNode->GetParentTM(0));
	Point3 pos;
	Quat rot;
	Point3 scl;

	AffineParts ap;
	decomp_affine(pivot, &ap);
	pos = ap.t;
	SwapFloat(pos.y, pos.z);

	AngAxis temprot(pivot);
	SwapFloat(temprot.axis.y, temprot.axis.z);
	rot = Quat(temprot);

	scl = ap.k;
	SwapFloat(scl.y, scl.z);

	// 스케일은 나중에 하는걸로
	_ftprintf(m_pStream, L"World_Position %10.4f %10.4f %10.4f\n", pos.x, pos.y, pos.z);
	_ftprintf(m_pStream, L"World_Rotation %10.4f %10.4f %10.4f %10.4f\n", rot.x, rot.y, rot.z, rot.w);
	_ftprintf(m_pStream, L"World_Scale %10.4f %10.4f %10.4f\n", scl.x, scl.y, scl.z);
}

void MOBJWriter::ExportGeometry(INode * pNode)
{
	Modifier* skinMod = FindModifyer(pNode, SKIN_CLASSID);
	if (skinMod = nullptr)
	{
		ExportMesh(pNode);
	}
	else
	{
		ExportSkinningMesh(pNode);
	}
}

Point3 MOBJWriter::GetTangetVector(Point3 p0, Point3 p1, Point3 p2, Point3 t0, Point3 t1, Point3 t2, Point3 n1)
{
	Point2 uv0 = Point2(t0.x, t0.y);
	Point2 uv1 = Point2(t1.x, t1.y);
	Point2 uv2 = Point2(t2.x, t2.y);
	Point3 vEdge1 = p1 - p0;
	Point3 vEdge2 = p2 - p0;
	vEdge1 = vEdge1.Normalize();
	vEdge2 = vEdge2.Normalize();
	// UV delta
	Point2 deltaUV1 = uv1 - uv0;
	Point2 deltaUV2 = uv2 - uv0;
	deltaUV1 = deltaUV1.Normalize();
	deltaUV2 = deltaUV2.Normalize();

	Point3 vTangent;
	float scale = (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);
	float fDet = 1.0f / scale;
	if (fabsf(fDet) < 1e-6f)
	{
		vTangent = Point3(1.0f, 0.0f, 0.0f);
	}
	else
	{
		vTangent = (vEdge1 * deltaUV2.y - vEdge2 * deltaUV1.y) * fDet;
	}
	vTangent = vTangent.Normalize();
	return vTangent;
}

BOOL MOBJWriter::TMNegParity(Matrix3 & m)
{
	return (DotProd(CrossProd(m.GetRow(0), m.GetRow(1)), m.GetRow(2)) < 0.0) ? 1 : 0;
}

void MOBJWriter::ExportMesh(INode * pNode)
{
	// ------------------------------------------------------------------------------------------------------------
	//	맥스에서는 4 x 3 행렬(Matrix3)을 사용한다.
	// ------------------------------------------------------------------------------------------------------------
	//Matrix3 tm = pNode->GetObjTMAfterWSM(0);
	Matrix3 tm = pNode->GetNodeTM(0);
	BOOL negScale = TMNegParity(tm);
	bool deleteit = false;
	Inverse(tm, tm);
	AffineParts ap;
	decomp_affine(tm, &ap);
	Quat inversRot = ap.q;
	inversRot.MakeMatrix(tm);
	//pNode->Scale();
	TriObject* tri = GetTriObjectFromNode(pNode, 0, deleteit);
	if (tri == nullptr)
	{
		return;
	}
	Mesh* mesh = &tri->GetMesh();

	vector<MVERTEX>	m_VertexList;
	vector<DWORD>		m_IndexList;

	if (mesh)
	{
		// ------------------------------------------------------------------------------------------------------------
		//	버텍스, 노말, uv버텍스 리스트들을 얻음
		// ------------------------------------------------------------------------------------------------------------
		mesh->buildNormals();
		vector<Point3> VList;
		vector<Point3> TList;
		vector<Point3> NList;
		VList.resize(mesh->numVerts);
		TList.resize(mesh->numTVerts);
		for (int i = 0; i < mesh->numVerts; i++)
		{
			Point3 V = mesh->verts[i];
			SwapFloat(V.y, V.z);
			VList[i] = V;
		}
		for (int i = 0; i < mesh->numTVerts; i++)
		{
			Point3 T = mesh->tVerts[i];
			T.y = 1.000f - T.y;
			TList[i] = T;
		}
		// ------------------------------------------------------------------------------------------------------------
		//	페이스 인덱싱
		// ------------------------------------------------------------------------------------------------------------
		vector<FaceIndex> FaceList;
		//vector<int[3][3]> FaceList;
		//GetParentTM
		Matrix3 itm = pNode->GetObjectTM(0);
		Inverse(itm, itm);
		for (int i = 0; i < mesh->getNumFaces(); i++)
		{
			// --------------------------------------------------------------------------------------------------------
			//	버텍스와 T버텍스 인덱싱
			// --------------------------------------------------------------------------------------------------------
			FaceIndex iFace;
			Face fv = mesh->faces[i];
			TVFace ft = mesh->tvFace[i];
			for (int j = 0; j < 3; j++)
			{
				iFace.m_Face[j].x = fv.v[j];
				iFace.m_Face[j].y = ft.t[j];
			}
			// --------------------------------------------------------------------------------------------------------
			//	Normal인덱싱
			// --------------------------------------------------------------------------------------------------------
			for (int j = 0; j < 3; j++)
			{
				int vert = fv.getVert(j);
				Point3 vn = GetVertexNormal(mesh, i, mesh->getRVertPtr(vert));
				SwapFloat(vn.y, vn.z);
				iFace.m_Face[j].z = CheckVertexNormal(NList, vn);
			}
			FaceList.push_back(iFace);
		}
		// ------------------------------------------------------------------------------------------------------------
		//	인덱스를 토대로 P3Vertex와 index를 생성
		// ------------------------------------------------------------------------------------------------------------
		for (int i = 0; i < FaceList.size(); i++)
		{
			MVERTEX newdata[3];
			newdata[0].p = VList[FaceList[i].m_Face[0].x];
			newdata[0].t = TList[FaceList[i].m_Face[0].y];
			newdata[0].n = NList[FaceList[i].m_Face[0].z];
			newdata[1].p = VList[FaceList[i].m_Face[2].x];
			newdata[1].t = TList[FaceList[i].m_Face[2].y];
			newdata[1].n = NList[FaceList[i].m_Face[2].z];
			newdata[2].p = VList[FaceList[i].m_Face[1].x];
			newdata[2].t = TList[FaceList[i].m_Face[1].y];
			newdata[2].n = NList[FaceList[i].m_Face[1].z];
			newdata[0].tv = GetTangetVector(
			VList[FaceList[i].m_Face[0].x], VList[FaceList[i].m_Face[1].x], VList[FaceList[i].m_Face[2].x],
			TList[FaceList[i].m_Face[0].y], TList[FaceList[i].m_Face[1].y], TList[FaceList[i].m_Face[2].y],
			NList[FaceList[i].m_Face[0].z]);
			newdata[1].tv = GetTangetVector(
			VList[FaceList[i].m_Face[1].x], VList[FaceList[i].m_Face[2].x], VList[FaceList[i].m_Face[0].x],
			TList[FaceList[i].m_Face[1].y], TList[FaceList[i].m_Face[2].y], TList[FaceList[i].m_Face[0].y],
			NList[FaceList[i].m_Face[1].z]);
			newdata[2].tv = GetTangetVector(
			VList[FaceList[i].m_Face[2].x], VList[FaceList[i].m_Face[0].x], VList[FaceList[i].m_Face[1].x],
			TList[FaceList[i].m_Face[2].y], TList[FaceList[i].m_Face[0].y], TList[FaceList[i].m_Face[1].y],
			NList[FaceList[i].m_Face[2].z]);
			m_IndexList.push_back(CheckMVERTEX(m_VertexList, newdata[0]));
			m_IndexList.push_back(CheckMVERTEX(m_VertexList, newdata[1]));
			m_IndexList.push_back(CheckMVERTEX(m_VertexList, newdata[2]));
		}
		// ------------------------------------------------------------------------------------------------------------
		//	data를 토대로 출력
		// ------------------------------------------------------------------------------------------------------------
		_ftprintf(m_pStream, L"GEOMETRY_DATA\n");
		int iVCount = m_VertexList.size();
		_ftprintf(m_pStream, L"MVERTEX_COUNT ");
		_ftprintf(m_pStream, L"%d\n", iVCount);
		for (int i = 0; i < iVCount; i++)
		{
			_ftprintf(m_pStream, L"%10.4f %10.4f %10.4f", m_VertexList[i].p.x, m_VertexList[i].p.y, m_VertexList[i].p.z);
			_ftprintf(m_pStream, L"%10.4f %10.4f %10.4f", m_VertexList[i].t.x, m_VertexList[i].t.y, m_VertexList[i].t.z);
			_ftprintf(m_pStream, L"%10.4f %10.4f %10.4f", m_VertexList[i].n.x, m_VertexList[i].n.y, m_VertexList[i].n.z);
			_ftprintf(m_pStream, L"%10.4f %10.4f %10.4f\n", m_VertexList[i].tv.x, m_VertexList[i].tv.y, m_VertexList[i].tv.z);
		}
		int iICount = m_IndexList.size();
		_ftprintf(m_pStream, L"INDEX_COUNT ");
		_ftprintf(m_pStream, L"%d\n", iICount);
		if (negScale)
		{
			for (int i = 0; i < iICount; i = i + 3)
			{
				_ftprintf(m_pStream, L"%d ", m_IndexList[i]);
				_ftprintf(m_pStream, L"%d ", m_IndexList[i + 2]);
				_ftprintf(m_pStream, L"%d ", m_IndexList[i + 1]);
			}
		}
		else
		{
			for (int i = 0; i < iICount; i = i + 3)
			{
				_ftprintf(m_pStream, L"%d ", m_IndexList[i]);
				_ftprintf(m_pStream, L"%d ", m_IndexList[i + 1]);
				_ftprintf(m_pStream, L"%d ", m_IndexList[i + 2]);
			}
		}
		_ftprintf(m_pStream, L"\n");
		_ftprintf(m_pStream, L"GEOMETRY_DATA_END\n");
	}
	if (deleteit) delete tri;
}

void MOBJWriter::ExportBoundingbox(INode * pNode)
{
	Matrix3 tm = pNode->GetNodeTM(0);
	bool deleteit = false;
	TriObject* tri = GetTriObjectFromNode(pNode, 0, deleteit);
	if (tri == nullptr)
	{
		return;
	}
	Mesh* mesh = &tri->GetMesh();
	mesh->buildBoundingBox();
	Box3 box = mesh->getBoundingBox();
	Point3 min = box.pmin; //*Inverse(tm);
	Point3 max = box.pmax; //*Inverse(tm);
	SwapFloat(min.y, min.z);
	SwapFloat(max.y, max.z);
	_ftprintf(m_pStream, L"BOUNDING_BOX\n");
	_ftprintf(m_pStream, L"%10.4f %10.4f %10.4f", min.x, min.y, min.z);
	_ftprintf(m_pStream, L"%10.4f %10.4f %10.4f", max.x, max.y, max.z);
	_ftprintf(m_pStream, L"\n");
}

Modifier* MOBJWriter::FindModifyer(INode * pNode, Class_ID classID)
{
	Object* ObjectPtr = pNode->GetObjectRef();
	if (!ObjectPtr) return nullptr;

	while (ObjectPtr->SuperClassID() == GEN_DERIVOB_CLASS_ID && ObjectPtr)
	{
		IDerivedObject* DerivedObjectPtr = (IDerivedObject*)(ObjectPtr);
		int ModifierStack = 0;

		while (ModifierStack < DerivedObjectPtr->NumModifiers())
		{
			Modifier* ModifierPtr = DerivedObjectPtr->GetModifier(ModifierStack);
			if (ModifierPtr->ClassID() == classID)
			{
				return ModifierPtr;
			}
			ModifierStack++;
		}
		ObjectPtr = DerivedObjectPtr->GetObjRef();
	}
	return nullptr;
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
			return 2;
			break;
		default:
			break;
		}
	}
	return 0;
}

void MOBJWriter::ExportSkinningMesh(INode * pNode)
{
	Modifier* skinMod = FindModifyer(pNode, SKIN_CLASSID);
	if (skinMod == nullptr) return;
	//get the skin interface
	ISkin *skin = (ISkin *)skinMod->GetInterface(I_SKIN);
	ISkinContextData *skinData = skin->GetContextInterface(pNode);

	// --------------------------------------------------------------------------------------------
	//int iBoneCount = skin->GetNumBones();
	//for (int i = 0; i < iBoneCount; i++)
	//{
	//	Matrix3 temp;
	//	INode* bone = skin->GetBone(i);
	//	skin->GetBoneInitTM(bone, temp);

	//	Point3 pos;
	//	Quat rot;

	//	AffineParts ap;
	//	decomp_affine(temp, &ap);
	//	pos = ap.t;
	//	SwapFloat(pos.y, pos.z);

	//	AngAxis temprot(temp);
	//	SwapFloat(temprot.axis.y, temprot.axis.z);
	//	rot = Quat(temprot);
	//	// 스케일은 나중에 하는걸로
	//	_ftprintf(m_pStream, L"Skin_Bone_Name %s\n", bone->GetName());
	//	_ftprintf(m_pStream, L"Skin_Bone_Init_Position %10.4f %10.4f %10.4f\n", pos.x, pos.y, pos.z);
	//	_ftprintf(m_pStream, L"Skin_Bone_Init_Rotation %10.4f %10.4f %10.4f %10.4f\n", rot.x, rot.y, rot.z, rot.w);
	//}
	// --------------------------------------------------------------------------------------------



	vector<BonePoint> BPList;

	if (skin && skinData)
	{
		int numberOfPoints = skinData->GetNumPoints();
		for (int i = 0; i < numberOfPoints; i++) // 버텍스 수만큼
		{
			// 본의 갯수 만큼 데이터를 가져온다. //
			list<BoneData> Bonelist;
			int numOfWeights = skinData->GetNumAssignedBones(i); // 연결된 본의 갯수
			for (int j = 0; j < numOfWeights; j++)
			{
				int boneIndex = skinData->GetAssignedBone(i, j);
				INode * pBone = skin->GetBone(boneIndex);
				if (pBone == NULL)  break;
				MCHAR* nodeName = const_cast<MCHAR*>(pBone->GetName());
				float boneWeight = skinData->GetBoneWeight(i, j);
				BoneData temp;
				temp.id = nodeName;
				temp.weigth = boneWeight;
				Bonelist.push_back(temp);
			}
			// 본의 갯수가 4개 이상일 경우 상위 4개의 데이터만 가져와서 정규화를 해준다 //
			int iCount = numOfWeights;
			Bonelist.sort([](const BoneData& lhs, const BoneData& rhs) -> bool {
				return lhs.weigth > rhs.weigth;
			});
			if(numOfWeights > 4)
			{
				numOfWeights = 4;
			}
			BonePoint bpdata;
			ZeroMemory(&bpdata, sizeof(bpdata));
			bpdata.m_NumWeight = numOfWeights;
			for (int i = 0; i < numOfWeights; i++)
			{
				list<BoneData>::iterator temp = Bonelist.begin();
				for (int j = 0; j < i; j++)
				{
					temp++;
				}
				bpdata.m_Weight[i] = (*temp).weigth;
				bpdata.m_ID[i] = (*temp).id;
			}
			if (iCount > 4)
			{
				bpdata.m_Weight.Normalize(); // 정규화
			}
			for (int i = numOfWeights - 1; i > 0; i--)
			{
				if (bpdata.m_Weight[i] == 0)
				{
					bpdata.m_ID[i] = NULL;
					bpdata.m_NumWeight--;
				}
			}
			BPList.push_back(bpdata);
		}
	}
	// ------------------------------------------------------------------------------------------------------------
	//	맥스에서는 4 x 3 행렬(Matrix3)을 사용한다.
	// ------------------------------------------------------------------------------------------------------------
	//Matrix3 tm = pNode->GetObjTMAfterWSM(0);
	Matrix3 tm = pNode->GetNodeTM(0);
	BOOL negScale = TMNegParity(tm);
	bool deleteit = false;
	Inverse(tm, tm);
	AffineParts ap;
	decomp_affine(tm, &ap);
	Quat inversRot = ap.q;
	inversRot.MakeMatrix(tm);
	//pNode->Scale();
	TriObject* tri = GetTriObjectFromNode(pNode, 0, deleteit);
	if (tri == nullptr)
	{
		return;
	}
	Mesh* mesh = &tri->GetMesh();

	vector<CVERTEX>		m_VertexList;
	vector<DWORD>		m_IndexList;

	if (mesh)
	{
		// ------------------------------------------------------------------------------------------------------------
		//	버텍스, 노말, uv버텍스 리스트들을 얻음
		// ------------------------------------------------------------------------------------------------------------
		mesh->buildNormals();
		vector<Point3> VList;
		vector<Point3> TList;
		vector<Point3> NList;
		VList.resize(mesh->numVerts);
		TList.resize(mesh->numTVerts);
		for (int i = 0; i < mesh->numVerts; i++)
		{
			Point3 V = mesh->verts[i] * tm;
			SwapFloat(V.y, V.z);
			VList[i] = V;
		}
		for (int i = 0; i < mesh->numTVerts; i++)
		{
			Point3 T = mesh->tVerts[i];
			T.y = 1.000f - T.y;
			TList[i] = T;
		}
		// ------------------------------------------------------------------------------------------------------------
		//	페이스 인덱싱
		// ------------------------------------------------------------------------------------------------------------
		vector<FaceIndex> FaceList;
		//vector<int[3][3]> FaceList;
		//GetParentTM
		Matrix3 itm = pNode->GetObjectTM(0);
		Inverse(itm, itm);
		for (int i = 0; i < mesh->getNumFaces(); i++)
		{
			// --------------------------------------------------------------------------------------------------------
			//	버텍스와 T버텍스 인덱싱
			// --------------------------------------------------------------------------------------------------------
			FaceIndex iFace;
			Face fv = mesh->faces[i];
			TVFace ft = mesh->tvFace[i];
			for (int j = 0; j < 3; j++)
			{
				iFace.m_Face[j].x = fv.v[j];
				iFace.m_Face[j].y = ft.t[j];
			}
			// --------------------------------------------------------------------------------------------------------
			//	Normal인덱싱
			// --------------------------------------------------------------------------------------------------------
			for (int j = 0; j < 3; j++)
			{
				int vert = fv.getVert(j);
				Point3 vn = GetVertexNormal(mesh, i, mesh->getRVertPtr(vert));
				SwapFloat(vn.y, vn.z);
				iFace.m_Face[j].z = CheckVertexNormal(NList, vn);
			}
			FaceList.push_back(iFace);
		}
		// ------------------------------------------------------------------------------------------------------------
		//	인덱스를 토대로 P3Vertex와 index를 생성
		// ------------------------------------------------------------------------------------------------------------
		for (int i = 0; i < FaceList.size(); i++)
		{
			CVERTEX newdata[3];
			newdata[0].p = VList[FaceList[i].m_Face[0].x];
			newdata[0].t = TList[FaceList[i].m_Face[0].y];
			newdata[0].n = NList[FaceList[i].m_Face[0].z];
			newdata[0].bp = BPList[FaceList[i].m_Face[0].x];
			newdata[1].p = VList[FaceList[i].m_Face[2].x];
			newdata[1].t = TList[FaceList[i].m_Face[2].y];
			newdata[1].n = NList[FaceList[i].m_Face[2].z];
			newdata[1].bp = BPList[FaceList[i].m_Face[2].x];
			newdata[2].p = VList[FaceList[i].m_Face[1].x];
			newdata[2].t = TList[FaceList[i].m_Face[1].y];
			newdata[2].n = NList[FaceList[i].m_Face[1].z];
			newdata[2].bp = BPList[FaceList[i].m_Face[1].x];
			newdata[0].tv = GetTangetVector(
				VList[FaceList[i].m_Face[0].x], VList[FaceList[i].m_Face[1].x], VList[FaceList[i].m_Face[2].x],
				TList[FaceList[i].m_Face[0].y], TList[FaceList[i].m_Face[1].y], TList[FaceList[i].m_Face[2].y],
				NList[FaceList[i].m_Face[0].z]);
			newdata[1].tv = GetTangetVector(
				VList[FaceList[i].m_Face[1].x], VList[FaceList[i].m_Face[2].x], VList[FaceList[i].m_Face[0].x],
				TList[FaceList[i].m_Face[1].y], TList[FaceList[i].m_Face[2].y], TList[FaceList[i].m_Face[0].y],
				NList[FaceList[i].m_Face[1].z]);
			newdata[2].tv = GetTangetVector(
				VList[FaceList[i].m_Face[2].x], VList[FaceList[i].m_Face[0].x], VList[FaceList[i].m_Face[1].x],
				TList[FaceList[i].m_Face[2].y], TList[FaceList[i].m_Face[0].y], TList[FaceList[i].m_Face[1].y],
				NList[FaceList[i].m_Face[2].z]);
			m_IndexList.push_back(CheckCVERTEX(m_VertexList, newdata[0]));
			m_IndexList.push_back(CheckCVERTEX(m_VertexList, newdata[1]));
			m_IndexList.push_back(CheckCVERTEX(m_VertexList, newdata[2]));
		}
		// ------------------------------------------------------------------------------------------------------------
		//	data를 토대로 출력
		// ------------------------------------------------------------------------------------------------------------
		_ftprintf(m_pStream, L"GEOMETRY_DATA\n");
		int iVCount = m_VertexList.size();
		_ftprintf(m_pStream, L"CVERTEX_COUNT ");
		_ftprintf(m_pStream, L"%d\n", iVCount);
		for (int i = 0; i < iVCount; i++)
		{
			_ftprintf(m_pStream, L"%10.4f %10.4f %10.4f ", m_VertexList[i].p.x, m_VertexList[i].p.y, m_VertexList[i].p.z);
			_ftprintf(m_pStream, L"%10.4f %10.4f %10.4f ", m_VertexList[i].t.x, m_VertexList[i].t.y, m_VertexList[i].t.z);
			_ftprintf(m_pStream, L"%10.4f %10.4f %10.4f ", m_VertexList[i].n.x, m_VertexList[i].n.y, m_VertexList[i].n.z);
			_ftprintf(m_pStream, L"%10.4f %10.4f %10.4f ", m_VertexList[i].tv.x, m_VertexList[i].tv.y, m_VertexList[i].tv.z);
			_ftprintf(m_pStream, L"%d ", m_VertexList[i].bp.m_NumWeight);
			for (int j = 0; j < m_VertexList[i].bp.m_NumWeight; j++)
			{
				ExportName(m_VertexList[i].bp.m_ID[j]);
				_ftprintf(m_pStream, L" %10.4f ", m_VertexList[i].bp.m_Weight[j]);
			}
			_ftprintf(m_pStream, L"\n");
		}
		int iICount = m_IndexList.size();
		_ftprintf(m_pStream, L"INDEX_COUNT ");
		_ftprintf(m_pStream, L"%d\n", iICount);
		if (negScale)
		{
			for (int i = 0; i < iICount; i = i + 3)
			{
				_ftprintf(m_pStream, L"%d ", m_IndexList[i]);
				_ftprintf(m_pStream, L"%d ", m_IndexList[i + 2]);
				_ftprintf(m_pStream, L"%d ", m_IndexList[i + 1]);
			}
		}
		else
		{
			for (int i = 0; i < iICount; i = i + 3)
			{
				_ftprintf(m_pStream, L"%d ", m_IndexList[i]);
				_ftprintf(m_pStream, L"%d ", m_IndexList[i + 1]);
				_ftprintf(m_pStream, L"%d ", m_IndexList[i + 2]);
			}
		}
		_ftprintf(m_pStream, L"\n");
		_ftprintf(m_pStream, L"GEOMETRY_DATA_END\n");
	}
	if (deleteit) delete tri;
}

void MOBJWriter::ExportMaterial(INode * pNode)
{
	Mtl* pMtl = pNode->GetMtl();
	if (pMtl)
	{
		GetTexture(pMtl);
	}
}

void MOBJWriter::ExportAnimationKeys(INode * pNode)
{
	// 스케일은 나중에 추가
	Control* pC = pNode->GetTMController()->GetPositionController();
	Control* rC = pNode->GetTMController()->GetRotationController();
	int iNumPos = pC->NumKeys();
	int iNumRot = rC->NumKeys();
	if (iNumPos + iNumRot == 0) { return; }
	float TickPerSecond = 4800.00f;
	TimeValue start = m_pMax->GetAnimRange().Start();
	TimeValue end = m_pMax->GetAnimRange().End();
	vector<Point3>	poslist;
	vector<int>	postime;
	vector<Quat>	rotlist;
	vector<int>	rottime;

	_ftprintf(m_pStream, L"ANIMATION_KEY\n");
	// ------------ 스타트 키 추가 ------------
	Matrix3 CurTm = pNode->GetNodeTM(start)  * Inverse(pNode->GetParentTM(start));
	AffineParts ap;
	decomp_affine(CurTm, &ap);
	Point3 pos = ap.t;
	SwapFloat(pos.y, pos.z);
	poslist.push_back(pos);
	postime.push_back(start);
	AngAxis temprot(CurTm);
	SwapFloat(temprot.axis.y, temprot.axis.z);
	Quat rot(temprot);
	rotlist.push_back(rot);
	rottime.push_back(start);
	// ------------ 키 정보 추가 ------------
	for (int i = 0; i < iNumPos; i++)
	{
		TimeValue t = pC->GetKeyTime(i);
		if (t > end)
		{
			if (postime[postime.size() - 1] != end)
			{
				Matrix3 CurTm = pNode->GetNodeTM(end) * Inverse(pNode->GetParentTM(end));
				AffineParts ap;
				decomp_affine(CurTm, &ap);
				Point3 pos = ap.t;
				SwapFloat(pos.y, pos.z);
				poslist.push_back(pos);
				postime.push_back(end);
			}
			break;
		}
		if (t > start)
		{
			Matrix3 CurTm = pNode->GetNodeTM(t) * Inverse(pNode->GetParentTM(t));
			AffineParts ap;
			decomp_affine(CurTm, &ap);
			Point3 pos = ap.t;
			SwapFloat(pos.y, pos.z);
			poslist.push_back(pos);
			postime.push_back(t);
		}
	}
	for (int i = 0; i < iNumRot; i++)
	{
		TimeValue t = rC->GetKeyTime(i);
		if (t > end)
		{
			if (rottime[rottime.size() - 1] != end)
			{
				Matrix3 CurTm = pNode->GetNodeTM(end) * Inverse(pNode->GetParentTM(end));
				AngAxis temprot(CurTm);
				SwapFloat(temprot.axis.y, temprot.axis.z);
				Quat rot(temprot);
				rotlist.push_back(rot);
				rottime.push_back(end);
			}
			break;
		}
		if (t > start)
		{
			Matrix3 CurTm = pNode->GetNodeTM(t) * Inverse(pNode->GetParentTM(t));
			AngAxis temprot(CurTm);
			SwapFloat(temprot.axis.y, temprot.axis.z);
			Quat rot(temprot);
			rotlist.push_back(rot);
			rottime.push_back(t);
		}
	}
	// ------------ 정보 추출 ------------
	iNumPos = poslist.size();
	iNumRot = rotlist.size();
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
	_ftprintf(m_pStream, L"ANIMATION_END\n");
}

void MOBJWriter::GetTexture(Mtl * pMtl)
{
	int iNumMap = pMtl->NumSubTexmaps();
	_ftprintf(m_pStream, L"MATERIAL_LIST\n");
	for (int i = 0; i < iNumMap; i++)
	{
		Texmap* tex = pMtl->GetSubTexmap(i);
		if (tex)
		{
			if (tex->ClassID() == Class_ID(BMTEX_CLASS_ID, 0X00))
			{
				TSTR MapName = ((BitmapTex*)(tex))->GetMapName();
				if (MapName)
				{
					_ftprintf(m_pStream, L"MAP_ID %d %s\n", i, MapName);
					// 출력
				}
			}
		}
	}
	_ftprintf(m_pStream, L"MATERIAL_LIST_END\n");
}

void MOBJWriter::SwapFloat(float & data1, float & data2)
{
	float temp = data1;
	data1 = data2;
	data2 = temp;
}

Point3 MOBJWriter::GetVertexNormal(Mesh * mesh, int faceNo, RVertex * rv)
{
	Face* f = &mesh->faces[faceNo];
	DWORD smGroup = f->smGroup;
	int numNormals = 0;
	Point3 vertexNormal;

	if (rv->rFlags & SPECIFIED_NORMAL)
	{
		// -----------------------------------------------------------
		//	
		// -----------------------------------------------------------
		vertexNormal = rv->rn.getNormal();
	}
	else if ((numNormals = rv->rFlags & NORCT_MASK) != 0 && smGroup)
	{
		if (numNormals == 1)
		{
			vertexNormal = rv->rn.getNormal();
		}
		else
		{
			for (int i = 0; i < numNormals; i++) 
			{
				if (rv->ern[i].getSmGroup() & smGroup)
				{
					vertexNormal = rv->ern[i].getNormal();
				}
			}
		}
	}
	else
	{
		vertexNormal = mesh->getFaceNormal(faceNo);
	}
	return vertexNormal;
}

UINT MOBJWriter::CheckVertexNormal(vector<Point3>& list, Point3 data)
{
	for(int i = 0; i < list.size(); i++)
	{
		if (list[i].Equals(data, 1E-3F))
		{
			return i;
		}
	}
	list.push_back(data);
	return list.size() - 1;
}

bool MOBJWriter::Export()
{
	_wfopen_s(&m_pStream, m_filename.c_str(), L"w");
	// -------------------------------------------------------------------------------
	for (int i = 0; i < m_ObjectList.size(); i++)
	{
		int iType = SearchType(m_ObjectList[i]);
		switch (iType)
		{
		case 1:
			_ftprintf(m_pStream, L"TYPE BONE\n");
			break;
		case 2:
			_ftprintf(m_pStream, L"TYPE HELPER\n");
			break;
		default:
			break;
		}
		_ftprintf(m_pStream, L"OBJECT_NAME ");
		//_ftprintf(m_pStream, L"%s\n", m_ObjectList[i]->GetName());
		ExportName(m_ObjectList[i]->GetName());
		_ftprintf(m_pStream, L"\n");
		ExportParent(m_ObjectList[i]);
		ExportMatrix(m_ObjectList[i]);
		switch (iType)
		{
		case 1:
			ExportBoundingbox(m_ObjectList[i]);
			break;
		default:
			break;
		}
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
