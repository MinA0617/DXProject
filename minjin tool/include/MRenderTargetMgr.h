#pragma once
#include "MRenderTarget.h"
#include "M2DScreen.h"

class MRenderTargetMgr : public MSingleton<MRenderTargetMgr>
{
	friend class MSingleton<MRenderTargetMgr>;
public:
	vector<MRenderTarget*>	m_RTList;
	vector<M2DScreen*>		m_SCList;
	int						m_iOldHeight;
	int						m_iOldWidth;
public:
	int CreateNewRenderTarget(UINT width, UINT heigth);
	int CreateNewScreen(UINT width, UINT heigth);
	bool Resize();
	bool Init();
	bool Frame();
	bool PreRender();
	bool Render();
	bool PostRender();
	bool Release();
private:
	MRenderTargetMgr();
public:
	~MRenderTargetMgr();
};

#define I_RenderTargetMgr MRenderTargetMgr::GetInstance()