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

class MOBJWriter
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
	void				SwapFloat(float &data1, float &data2);
	int					SearchType(INode* pNode);
	int					mmin(int a, int b) { if (a > b) { return b; } else { return a; } };
// ---------------------------------------------------------------------------------------------------------------------------
	bool				Export();
	void				ExportName(const MCHAR* name);
	void				ExportAnimationKeys(INode* pNode);
public:
	MOBJWriter();
	~MOBJWriter();
};

