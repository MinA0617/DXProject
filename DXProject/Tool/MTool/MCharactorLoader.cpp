#include "stdafx.h"
#include "MCharactorLoader.h"


M_STR MCharactorLoader::AnotherName(M_STR oldpath, M_STR ext)
{
	M_STR str = oldpath;
	str += L" 파일을 찾을 수 없습니다. 경로를 재설정 해주십시오.";
	MessageBoxW(g_hWnd, str.c_str(), L"Error", MB_OK);
	OPENFILENAME    ofn;
	TCHAR           szFile[MAX_PATH] = { 0, };
	TCHAR			szFileTitle[MAX_PATH] = { 0, };
	static TCHAR    *szFilter;

	TCHAR lpCurBuffer[256] = { 0, };
	GetCurrentDirectory(256, lpCurBuffer);

	ZeroMemory(&ofn, sizeof(OPENFILENAME));
	_tcscpy_s(szFile, L"*.");
	_tcscpy_s(szFile, ext.c_str());
	_tcscpy_s(szFile, L"\0");

	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = GetActiveWindow();
	ofn.lpstrFilter = szFilter;
	ofn.lpstrCustomFilter = NULL;
	ofn.nMaxCustFilter = 0L;
	ofn.nFilterIndex = 1;
	ofn.lpstrFile = szFile;
	ofn.nMaxFile = MAX_PATH;
	ofn.lpstrFileTitle = szFileTitle;
	ofn.nMaxFileTitle = MAX_PATH;
	ofn.lpstrInitialDir = _T("../../data/");
	ofn.lpstrTitle = L"asd";
	ofn.Flags = OFN_EXPLORER | OFN_ALLOWMULTISELECT;
	ofn.nFileOffset = 0;
	ofn.nFileExtension = 0;
	ofn.lpstrDefExt = ext.c_str();

	if (!GetOpenFileName(&ofn))
	{
		return false;
	}
	TCHAR* load = _tcstok(szFile, _T("\n"));
	M_STR dir = szFile;

	SetCurrentDirectory(lpCurBuffer);
	return dir;

}

M_STR MCharactorLoader::SKTLoad(M_STR path)
{
	if (!I_Parser.Load(path))
	{
		while (1)
		{
			M_STR newpath = AnotherName(path, L"SKT");
			if (I_Parser.Load(newpath))
			{
				TCHAR Drive[MAX_PATH] = { 0, };
				TCHAR Dir[MAX_PATH] = { 0, };
				TCHAR Name[MAX_PATH] = { 0, };
				TCHAR Ext[MAX_PATH] = { 0, };
				_tsplitpath_s(newpath.c_str(), Drive, Dir, Name, Ext);
				M_STR name = Name;
				return name;
			}
		}
	}
	TCHAR Drive[MAX_PATH] = { 0, };
	TCHAR Dir[MAX_PATH] = { 0, };
	TCHAR Name[MAX_PATH] = { 0, };
	TCHAR Ext[MAX_PATH] = { 0, };
	_tsplitpath_s(path.c_str(), Drive, Dir, Name, Ext);
	M_STR name = Name;
	return name;
}

bool MCharactorLoader::BBLoad(M_STR path, M_STR sktname)
{
	if (!I_Parser.Load_BB(path, I_SkeletonMgr[sktname]))
	{
		while (1)
		{
			M_STR newpath = AnotherName(path, L"OBJ");
			if (I_Parser.Load_BB(path, I_SkeletonMgr[sktname]))
			{
				return true;
			}
		}
	}
	return true;
}

bool MCharactorLoader::OBJLoad(M_STR path, M_STR sktname)
{
	vector<M_STR> namelist;
	if (!I_Parser.Load(path, namelist, I_SkeletonMgr[sktname]))
	{
		while (1)
		{
			M_STR newpath = AnotherName(path, L"OBJ");
			if (I_Parser.Load(path, namelist, I_SkeletonMgr[sktname]))
			{
				return true;
			}
		}
	}
	return true;
}

bool MCharactorLoader::KEYLoad(M_STR path)
{
	if (!I_Parser.Load(path))
	{
		while (1)
		{
			M_STR newpath = AnotherName(path, L"KEY");
			if (I_Parser.Load(path))
			{
				return true;
			}
		}
	}
	return true;
}

bool MCharactorLoader::CreatUnit(M_STR sktname, M_STR * path, int count)
{
	if (I_3DObjectMgr.CreateUnit(sktname, sktname, path, count) == nullptr)
	{
		return false;
	}
	return true;
}

bool MCharactorLoader::Load(M_STR path)
{
	int iIsBox, ObjCount, KeyCount, UnitMeshCount;
	M_STR sktname;
	TCHAR buffer[MAX_PATH] = { 0, };
	FILE* m_pStream;
	_wfopen_s(&m_pStream, path.c_str(), L"r");
	{
		fscanf_s(m_pStream, "%S\n", buffer, MAX_PATH);
		M_STR loadpath = buffer;
		sktname = SKTLoad(loadpath);
	}
	fscanf_s(m_pStream, "%d\n", iIsBox);
	if (iIsBox)
	{
		fscanf_s(m_pStream, "%S\n", buffer, MAX_PATH);
		M_STR loadpath = buffer;
		BBLoad(loadpath, sktname);
	}
	fscanf_s(m_pStream, "%d\n", ObjCount);
	for (int i = 0; i < ObjCount; i++)
	{
		fscanf_s(m_pStream, "%S\n", buffer, MAX_PATH);
		M_STR loadpath = buffer;
		OBJLoad(loadpath, sktname);
	}
	fscanf_s(m_pStream, "%d\n", KeyCount);
	for (int i = 0; i < KeyCount; i++)
	{
		fscanf_s(m_pStream, "%S\n", buffer, MAX_PATH);
		M_STR loadpath = buffer;
		KEYLoad(loadpath);
	}
	fscanf_s(m_pStream, "%d\n", UnitMeshCount);
	vector<M_STR> meshlist;
	if (UnitMeshCount == 0) return false;
	for (int i = 0; i < UnitMeshCount; i++)
	{
		fscanf_s(m_pStream, "%S\n", buffer, MAX_PATH);
		M_STR loadpath = buffer;
		meshlist.push_back(loadpath);
	}
	CreatUnit(sktname, &meshlist.at(0), UnitMeshCount);
	fclose(m_pStream);
	return true;
}

MCharactorLoader::MCharactorLoader()
{
}


MCharactorLoader::~MCharactorLoader()
{
}
