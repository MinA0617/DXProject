#include "stdafx.h"
#include "MTileing.h"


bool MTileing::SetTile()
{
	if (m_iTextureID >= I_3DObjectMgr.m_InWorldFiled->ground->m_TextureList.size()) return false;
	MMapNode* result = MSelect::PickTile();
	if(!result) return false;
	MTexture* texture = I_TextureMgr[I_3DObjectMgr.m_InWorldFiled->ground->m_TextureList[m_iTextureID]];
	if (!texture) return false;
	result->SetTexture(texture, DIFFUSE, m_iCurLayer);
	return true;
}

MTileing::MTileing()
{
	m_iTextureID = -1;
	m_iCurLayer = 0;
}


MTileing::~MTileing()
{
}
