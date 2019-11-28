#include "stdafx.h"
#include "MTileing.h"


bool MTileing::SetTile()
{
	if (m_iTextureID >= I_3DObjectMgr.m_InWorldFiled->ground->m_TextureList.size()) return false;
	MMapNode* result = MSelect::PickMapNode();
	if (result)
	{
		result->SetTexture(m_iTextureID, DIFFUSE, m_iCurLayer);
		return true;
	}
	return false;
}

MTileing::MTileing()
{
	m_iTextureID = -1;
	m_iCurLayer = 0;
}


MTileing::~MTileing()
{
}
