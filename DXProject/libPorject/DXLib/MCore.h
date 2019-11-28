#pragma once

#include "MWindow.h"
#include "MDevice.h"
#include "MTimer.h"
#include "MInput.h"
#include "MSoundMgr.h"
#include "MCreate.h"
#include "MParser.h"
#include "MLight.h"
#include "MCollision.h"
#include "MSelect.h"
#include "MFunction.h"

class MCore : public MWindow
{
public:
	MCore();
	virtual ~MCore();
	bool	 m_bDebug;
private:
	virtual bool Init();
	virtual bool Frame();
	virtual bool Render();
	virtual bool Release();
public:
	bool MCoreInit();
	bool MCoreFrame();
	bool MCoreRender();
	bool MCoreRelease();
	void MCoreMsgProc(MSG msg);
	bool ResizeDevice(UINT width, UINT height);
public:
	bool PreRender();
	bool PostRender();
	bool DebugRender();
public:
	bool Run();
public:

};
