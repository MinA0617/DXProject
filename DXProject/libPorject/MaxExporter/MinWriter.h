#pragma once
#include <string>
#include <iostream>
#include <vector>
#include <list>
#include "Standard.h"

using namespace std;
// -------------------------------------------------------------------------------
//	INode의 메서드
//	GetName() - 이름 반환
//	NumberOfChildren() - 노드의 자식의 수를 찾아준다.
//	GetChildNode(int i) - i번째의 자식 노드 리턴.
//	EvalWorldState(int tick) - tick에 해당하는 노드의 상태를 가져온다.
// -------------------------------------------------------------------------------
//	Mesh 관련
//	GetObjTMAfterWSM(int tick) - tick에 해당하는 월드행렬 반환
//	
// -------------------------------------------------------------------------------

class MinWriter
{
private:
	FILE*				m_pStream;
	Interface*			m_pMax;
	wstring				m_filename;
	INode*				m_pRootNode;
	vector<INode*>		m_ObjectList;
public:
	void				Set(const TCHAR* name, Interface* pMax);
	void				PreProcess(INode* pNode);
	void				AddObject(INode* pNode);
	UINT				CheckMVERTEX(vector<MVERTEX> &list, MVERTEX data);
	UINT				CheckCVERTEX(vector<CVERTEX> &list, CVERTEX data);
	TriObject*			GetTriObjectFromNode(INode* pNode, TimeValue tick, bool& deleteit);
	void				GetTexture(Mtl* pMtl);
	void				SwapFloat(float &data1, float &data2);
	Point3				GetVertexNormal(Mesh* mesh, int faceNo, RVertex* rv);
	UINT				CheckVertexNormal(vector<Point3> &list, Point3 data);
	Point3				GetTangetVector(Point3 p0, Point3 p1, Point3 p2, Point3 t0, Point3 t1, Point3 t2, Point3 n1);
	BOOL				TMNegParity(Matrix3 &m);
	Modifier*			FindModifyer(INode * pNode, Class_ID classID);
	int					SearchType(INode* pNode);
	int					mmin(int a, int b) { if (a > b) { return b; } else { return a; } };
// ---------------------------------------------------------------------------------------------------------------------------
	void				ExportParent(INode* pNode);
	void				ExportMatrix(INode* pNode);
	void				ExportGeometry(INode* pNode);
	void				ExportMesh(INode* pNode);
	void				ExportSkinningMesh(INode* pNode);
	void				ExportSkinningMesh2(INode* pNode);
	void				ExportMaterial(INode* pNode);
	void				ExportAnimationKeys(INode* pNode);
	bool				Export();
public:
	MinWriter();
	~MinWriter();
};

