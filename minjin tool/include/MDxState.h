#pragma once
#include "MStd.h"

#define MAX_BS 1
#define MAX_RS 3
#define MAX_SS 1
#define MAX_DSS 2

enum BlendState { MAlphaBlend = 0 };
enum RasterizerState { MWireFrame = 0, MSolidFrame = 1, MNoneCulling = 2};
enum SamplerState { MWrapLinear = 0 };
enum DepthStencilState { MDepthEnable = 0, MDepthDisable = 1 };

class MDxState : public MSingleton<MDxState>
{
public:
	friend class MSingleton<MDxState>;
public:
	bool						m_bIsSolid;
	//////////////////////////////////////////
	ID3D11BlendState*			m_BlendState[MAX_BS];
	ID3D11RasterizerState*		m_RasterizerState[MAX_RS];
	ID3D11SamplerState*			m_SamplerState[MAX_SS];
	ID3D11DepthStencilState*	m_DepthStencilState[MAX_DSS];
	//////////////////////////////////////////

	bool BS_Set(BlendState BlendStateID, const FLOAT fBlendFactor[] = 0, UINT iMask = 0xffffffff)
	{
		if (BlendStateID > MAX_BS)
		{
			return false;
		}
		g_pImmediateContext->OMSetBlendState(m_BlendState[BlendStateID], fBlendFactor, iMask);
		return true;
	};
	////OMSetBlendState(블랜드스테이트, 블랜드팩터에서 사용할 RGBA상수값, 멀티샘플랜더타겟의 마스킹 타겟)
	bool RS_Set(RasterizerState RasterizerStateID)
	{
		if (RasterizerStateID == MSolidFrame)
		{
			m_bIsSolid = true;
		}
		if (RasterizerStateID == MWireFrame)
		{
			m_bIsSolid = false;
		}
		if (RasterizerStateID > MAX_RS)
		{
			return false;
		}
		g_pImmediateContext->RSSetState(m_RasterizerState[RasterizerStateID]);
		return true;
	};
	bool SS_Set(SamplerState SamplerStateID, UINT iSlot = 0, UINT iArray = 1)
	{
		if (SamplerStateID > MAX_SS)
		{
			return false;
		}
		g_pImmediateContext->PSSetSamplers(iSlot, iArray, &m_SamplerState[SamplerStateID]);
		return true;
	};
	bool DSS_Set(DepthStencilState DepthStencilStateID,	UINT iRef = 1)
	{
		if (DepthStencilStateID > MAX_DSS)
		{
			return false;
		}
		g_pImmediateContext->OMSetDepthStencilState(m_DepthStencilState[DepthStencilStateID], iRef);
		return true;
	}
	bool Init();
	bool Release();
private:
	MDxState();
public:
	~MDxState();
};

#define I_DxState MDxState::GetInstance()