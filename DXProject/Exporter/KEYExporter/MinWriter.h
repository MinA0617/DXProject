#pragma once
#include <string>
#include <iostream>
#include <vector>
#include <list>
#include "Standard.h"

using namespace std;
// -------------------------------------------------------------------------------
//	INode�� �޼���
//	GetName() - �̸� ��ȯ
//	NumberOfChildren() - ����� �ڽ��� ���� ã���ش�.
//	GetChildNode(int i) - i��°�� �ڽ� ��� ����.
//	EvalWorldState(int tick) - tick�� �ش��ϴ� ����� ���¸� �����´�.
// -------------------------------------------------------------------------------
//	Mesh ����
//	GetObjTMAfterWSM(int tick) - tick�� �ش��ϴ� ������� ��ȯ
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

