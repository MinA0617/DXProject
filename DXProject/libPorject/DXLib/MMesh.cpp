#include "MMesh.h"



MMesh::MMesh()
{
}


MMesh::~MMesh()
{
}

bool MMesh::Release()
{
	VertexData.clear();
	IndexData.clear();
	SAFE_RELEASE(m_pVertexBuffer);
	SAFE_RELEASE(m_pIndexBuffer);
	return true;
}