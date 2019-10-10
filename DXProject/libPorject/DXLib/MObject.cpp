#include "MObject.h"



MObject::MObject()
{
	MaterialID = I_MaterialMgr.CreateNewMaterial();
}


MObject::~MObject()
{
}

MMaterial * MObject::GetMatarial()
{
	return I_MaterialMgr[MaterialID];
}