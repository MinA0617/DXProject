//#include "MCreate.h"
//using namespace MCreate;
//
//UINT Counter::iCount_Box = 0;
//UINT Counter::iCount_Plane = 0;
//
//M3DModel* Geometry::Box(float Width, float Height, float Length, int WidthSegs, int HeightSegs, int LengthSegs)
//{
//	//// 박스 생성 함수 ////
//	//// 피봇은 하단 중앙 ////
//	// Length;
//	// Width;
//	// Height;
//	// LengthSegs;
//	// WidthSegs;
//	// HeightSegs;
//	return nullptr;
//}
//
//M3DModel* Geometry::Plane(float Width, float Length, int WidthSegs, int LengthSegs)
//{
//	//// 피봇은 중앙 ////
//	// 최대크기는 중앙으로부터 Length
//	// 시작지점은 -Length/2, -Width/2
//	// 예외처리
//	if (Width <= 0 || Length <= 0 || WidthSegs <= 0 || LengthSegs <= 0)
//	{
//		return nullptr;
//	}
//
//
//
//	MMesh* PlaneElement = new MMesh;
//
//	// x스탭 = Width/WidthSegs
//	for (int z = 0; z <= LengthSegs; z++)
//	{
//		for (int x = 0; x <= WidthSegs; x++)
//		{
//			MVERTEX newdata;
//			newdata.p = float3(-Width / 2 + (Width / WidthSegs * x), 0, Length / 2 - (Length / LengthSegs * z));
//			newdata.t = float3(x * 1.0000 / WidthSegs, z * 1.0000 / LengthSegs, 0);
//			newdata.n = float3(0.0, 1.0, 0.0);
//			PlaneElement->VertexData.push_back(newdata);
//		}
//	}
// 	for (int i = 0, j = 0, k = 0; i < LengthSegs * WidthSegs; i++)
//	{
//		int index = j;
//		PlaneElement->IndexData.push_back(index);
//		PlaneElement->IndexData.push_back(index + 1);
//		PlaneElement->IndexData.push_back(index + (WidthSegs + 1));
//		PlaneElement->IndexData.push_back(index + (WidthSegs + 1));
//		PlaneElement->IndexData.push_back(index + 1);
//		PlaneElement->IndexData.push_back(index + (WidthSegs + 2));
//		j++;
//		k++;
//		if (k == WidthSegs)
//		{
//			k = 0;
//			j++;
//		}
//	}
//	for (int i = 0; i < PlaneElement->IndexData.size()/3; i++)
//	{
//		MVERTEX* v0 = &PlaneElement->VertexData[PlaneElement->IndexData[i * 3 + 0]];
//		MVERTEX* v1 = &PlaneElement->VertexData[PlaneElement->IndexData[i * 3 + 1]];
//		MVERTEX* v2 = &PlaneElement->VertexData[PlaneElement->IndexData[i * 3 + 2]];
//		v0->tv = MMesh::GetTangetVector(v0->p, v1->p, v2->p, v0->t, v1->t, v2->t, v0->n);
//		v1->tv = MMesh::GetTangetVector(v1->p, v2->p, v0->p, v1->t, v2->t, v0->t, v1->n);
//		v2->tv = MMesh::GetTangetVector(v2->p, v0->p, v1->p, v2->t, v0->t, v1->t, v2->n);
//	}
//	PlaneElement->CreateBuffer();
//
//	M3DModel* Plane = new M3DModel;
//	Plane->Init();
//	M_STR name = L"Plane";
//	TCHAR temp[6] = { 0 };
//	_itot_s(Counter::iCount_Plane, temp, 10);
//	name += temp;
//	if (Plane->CreateObj(PlaneElement, name))
//	{
//		return Plane;
//	}
//	return nullptr;
//}