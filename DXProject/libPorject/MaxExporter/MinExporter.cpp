// MaxExporter.cpp : DLL 응용 프로그램을 위해 내보낸 함수를 정의합니다.
//
#include "stdafx.h"
#include "MinWriter.h"
#define MIN_EXPORT_CLASS_ID Class_ID(0x49213609, 0x6dd96044)

class MinExport : public SceneExport
{
public:
	MinWriter		m_MinWriter;
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
	MinExport() {};
	~MinExport() {};
};

class MinExporterClassDesc : public ClassDesc2
{
public:
	virtual int IsPublic() override
	{
		return TRUE;
	};
	virtual void* Create(BOOL /*loading = FALSE*/)
	{
		return new MinExport();
	}
	virtual const TCHAR *	ClassName()
	{
		return _T("Min_Exporter_version_1.0");
	}
	virtual Class_ID ClassID() override
	{
		return MIN_EXPORT_CLASS_ID;
	};
	virtual SClass_ID SuperClassID() override
	{
		return SCENE_EXPORT_CLASS_ID; 
	};
	virtual const TCHAR* Category()
	{
		return _T("MinExporter");
	}
	virtual const TCHAR* InternalName() override
	{
		return _T("MinExportClassDesc");
	};
	virtual HINSTANCE HInstance() override
	{
		return hInstance;
	};
};

ClassDesc2* GetExportDesc()
{
	static MinExporterClassDesc MinExporterDesc;
	return &MinExporterDesc;
}


int MinExport::ExtCount()
{
	return 1;
}

const MCHAR * MinExport::Ext(int n)
{

	return _T("min");
}

const MCHAR * MinExport::LongDesc()
{
	return _T("Min_Exporter_version_1.0");
}

const MCHAR * MinExport::ShortDesc()
{
	return _T("Min_Exporter");
}

const MCHAR * MinExport::AuthorName()
{
	return _T("Minjin_Kim");
}

const MCHAR * MinExport::CopyrightMessage()
{
	return _T("ATV_Corporation");
}

const MCHAR * MinExport::OtherMessage1()
{
	return _T("Hello");
}

const MCHAR * MinExport::OtherMessage2()
{
	return _T("Bye");
}

unsigned int MinExport::Version()
{
	// 버전. 100은 1.00 이라는 뜻
	return 100;
}

void MinExport::ShowAbout(HWND hWnd)
{
}

int MinExport::DoExport(const MCHAR * name, ExpInterface * ei, Interface * i, BOOL suppressPrompts, DWORD options)
{
	m_MinWriter.Set(name, i);
	m_MinWriter.Export();
	return TRUE;
}
