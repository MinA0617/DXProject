// dllmain.cpp : DLL 응용 프로그램의 진입점을 정의합니다.
#include "stdafx.h"
HINSTANCE hInstance;
extern ClassDesc2* GetExportDesc();

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
	hInstance = hModule;
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

__declspec(dllexport) const TCHAR* LibDescription()
{
	return _T("MinExporter_100");
}

__declspec(dllexport) int LibNumberClasses()
{
	return 1;
}

__declspec(dllexport) ClassDesc* LibClassDesc(int i)
{
	switch (i)
	{
	case 0:
	{
		return GetExportDesc();
		break;
	}
	default:
	{
		return 0;
	}
	}
}

__declspec(dllexport) ULONG LibVersion()
{
	return VERSION_3DSMAX;
}

__declspec(dllexport) int LibInitialize(void)
{
#pragma message(TODO("Perform initialization here."))
	return TRUE;
}

__declspec(dllexport) int LibShutdown(void)
{
#pragma message(TODO("Perform un-initialization here."))
	return TRUE;
}