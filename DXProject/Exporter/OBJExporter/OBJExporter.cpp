// MaxExporter.cpp : DLL 응용 프로그램을 위해 내보낸 함수를 정의합니다.
//
#include "stdafx.h"
#include "MinWriter.h"
#define KEY_EXPORT_CLASS_ID Class_ID(0x7ebf5c12, 0x73054714)

class OBJExport : public SceneExport
{
public:
	MOBJWriter		m_MinWriter;
	int				ExtCount();				//
	const MCHAR *	Ext(int n);				// 확장자
	const MCHAR *	LongDesc();				// 데스크 이름 긴 거
	const MCHAR *	ShortDesc();			// 데스크 이름 짧은 거
	const MCHAR *	AuthorName();			// 제작자 명
	const MCHAR *	CopyrightMessage();		// 회사 명
	const MCHAR *	OtherMessage1();		// ?
	const MCHAR *	OtherMessage2();		// ?
	unsigned int	Version();				// 버전
	void			ShowAbout(HWND hWnd);
	int				DoExport(const MCHAR *name, ExpInterface *ei, Interface *i, BOOL suppressPrompts = FALSE, DWORD options = 0);
public:
	OBJExport() {};
	~OBJExport() {};
};

class OBJExporterClassDesc : public ClassDesc2
{
public:
	virtual int IsPublic() override
	{
		return TRUE;
	};
	virtual void* Create(BOOL /*loading = FALSE*/)
	{
		return new OBJExport();
	}
	virtual const TCHAR *	ClassName()
	{
		return _T("Obj_Exporter_version_1.0");
	}
	virtual Class_ID ClassID() override
	{
		return KEY_EXPORT_CLASS_ID;
	};
	virtual SClass_ID SuperClassID() override
	{
		return SCENE_EXPORT_CLASS_ID; 
	};
	virtual const TCHAR* Category()
	{
		return _T("ObjExporter");
	}
	virtual const TCHAR* InternalName() override
	{
		return _T("ObjExportClassDesc");
	};
	virtual HINSTANCE HInstance() override
	{
		return hInstance;
	};
};

ClassDesc2* GetExportDesc()
{
	static OBJExporterClassDesc MinExporterDesc;
	return &MinExporterDesc;
}


int OBJExport::ExtCount()
{
	return 1;
}

const MCHAR * OBJExport::Ext(int n)
{

	return _T("obj");
}

const MCHAR * OBJExport::LongDesc()
{
	return _T("Obj_Exporter_version_1.0");
}

const MCHAR * OBJExport::ShortDesc()
{
	return _T("M::Obj_Exporter");
}

const MCHAR * OBJExport::AuthorName()
{
	return _T("Minjin_Kim");
}

const MCHAR * OBJExport::CopyrightMessage()
{
	return _T("ATV_Corporation");
}

const MCHAR * OBJExport::OtherMessage1()
{
	return _T("Hello");
}

const MCHAR * OBJExport::OtherMessage2()
{
	return _T("Bye");
}

unsigned int OBJExport::Version()
{
	// 버전. 100은 1.00 이라는 뜻
	return 100;
}

void OBJExport::ShowAbout(HWND hWnd)
{
}

int OBJExport::DoExport(const MCHAR * name, ExpInterface * ei, Interface * i, BOOL suppressPrompts, DWORD options)
{
	m_MinWriter.Set(name, i);
	m_MinWriter.Export();
	return TRUE;
}
