// MaxExporter.cpp : DLL 응용 프로그램을 위해 내보낸 함수를 정의합니다.
//
#include "stdafx.h"
#include "MinWriter.h"
#define SKT_EXPORT_CLASS_ID Class_ID(0x530d494c, 0x77ba735c)

class SKTExport : public SceneExport
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
	SKTExport() {};
	~SKTExport() {};
};

class SKTExporterClassDesc : public ClassDesc2
{
public:
	virtual int IsPublic() override
	{
		return TRUE;
	};
	virtual void* Create(BOOL /*loading = FALSE*/)
	{
		return new SKTExport();
	}
	virtual const TCHAR *	ClassName()
	{
		return _T("Skt_Exporter_version_1.0");
	}
	virtual Class_ID ClassID() override
	{
		return SKT_EXPORT_CLASS_ID;
	};
	virtual SClass_ID SuperClassID() override
	{
		return SCENE_EXPORT_CLASS_ID; 
	};
	virtual const TCHAR* Category()
	{
		return _T("SktExporter");
	}
	virtual const TCHAR* InternalName() override
	{
		return _T("SktExportClassDesc");
	};
	virtual HINSTANCE HInstance() override
	{
		return hInstance;
	};
};

ClassDesc2* GetExportDesc()
{
	static SKTExporterClassDesc MinExporterDesc;
	return &MinExporterDesc;
}


int SKTExport::ExtCount()
{
	return 1;
}

const MCHAR * SKTExport::Ext(int n)
{

	return _T("skt");
}

const MCHAR * SKTExport::LongDesc()
{
	return _T("Skt_Exporter_version_1.0");
}

const MCHAR * SKTExport::ShortDesc()
{
	return _T("M::Skt_Exporter");
}

const MCHAR * SKTExport::AuthorName()
{
	return _T("Minjin_Kim");
}

const MCHAR * SKTExport::CopyrightMessage()
{
	return _T("ATV_Corporation");
}

const MCHAR * SKTExport::OtherMessage1()
{
	return _T("Hello");
}

const MCHAR * SKTExport::OtherMessage2()
{
	return _T("Bye");
}

unsigned int SKTExport::Version()
{
	// 버전. 100은 1.00 이라는 뜻
	return 100;
}

void SKTExport::ShowAbout(HWND hWnd)
{
}

int SKTExport::DoExport(const MCHAR * name, ExpInterface * ei, Interface * i, BOOL suppressPrompts, DWORD options)
{
	m_MinWriter.Set(name, i);
	m_MinWriter.Export();
	return TRUE;
}
