// MaxExporter.cpp : DLL 응용 프로그램을 위해 내보낸 함수를 정의합니다.
//
#include "stdafx.h"
#include "MinWriter.h"
#define KEY_EXPORT_CLASS_ID Class_ID(0x4dea09cf, 0x7dd53957)

class KEYExport : public SceneExport
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
	KEYExport() {};
	~KEYExport() {};
};

class KEYExporterClassDesc : public ClassDesc2
{
public:
	virtual int IsPublic() override
	{
		return TRUE;
	};
	virtual void* Create(BOOL /*loading = FALSE*/)
	{
		return new KEYExport();
	}
	virtual const TCHAR *	ClassName()
	{
		return _T("Key_Exporter_version_1.0");
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
		return _T("KeyExporter");
	}
	virtual const TCHAR* InternalName() override
	{
		return _T("KeyExportClassDesc");
	};
	virtual HINSTANCE HInstance() override
	{
		return hInstance;
	};
};

ClassDesc2* GetExportDesc()
{
	static KEYExporterClassDesc MinExporterDesc;
	return &MinExporterDesc;
}


int KEYExport::ExtCount()
{
	return 1;
}

const MCHAR * KEYExport::Ext(int n)
{

	return _T("key");
}

const MCHAR * KEYExport::LongDesc()
{
	return _T("Key_Exporter_version_1.0");
}

const MCHAR * KEYExport::ShortDesc()
{
	return _T("M::Key_Exporter");
}

const MCHAR * KEYExport::AuthorName()
{
	return _T("Minjin_Kim");
}

const MCHAR * KEYExport::CopyrightMessage()
{
	return _T("ATV_Corporation");
}

const MCHAR * KEYExport::OtherMessage1()
{
	return _T("Hello");
}

const MCHAR * KEYExport::OtherMessage2()
{
	return _T("Bye");
}

unsigned int KEYExport::Version()
{
	// 버전. 100은 1.00 이라는 뜻
	return 100;
}

void KEYExport::ShowAbout(HWND hWnd)
{
}

int KEYExport::DoExport(const MCHAR * name, ExpInterface * ei, Interface * i, BOOL suppressPrompts, DWORD options)
{
	m_MinWriter.Set(name, i);
	m_MinWriter.Export();
	return TRUE;
}
