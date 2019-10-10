#pragma once

using M_STR = std::basic_string<TCHAR>;

struct float3
{
	union
	{
		struct { float x, y, z; };
		float v[3];
	};
	float3 operator - (float3 data)
	{
		float3 result;
		result.x = x - data.x;
		result.y = y - data.y;
		result.z = z - data.z;
		return result;
	};
	bool operator == (float3 data)
	{
		if (x == data.x && y == data.y && z == data.z)
		{
			return true;
		}
		return false;
	};
	bool operator != (float3 data)
	{
		if (x == data.x && y == data.y && z == data.z)
		{
			return false;
		}
		return true;
	};
	float3() {};
	float3(float fx, float fy, float fz)
	{
		x = fx;
		y = fy;
		z = fz;
	};
	float3(Point3 data)
	{
		x = data.x;
		y = data.y;
		z = data.z;
	};
};

struct float4
{
	union
	{
		struct { float x, y, z, w; };
		float v[4];
	};
	float4() {};
	float4(float fx, float fy, float fz, float fw)
	{
		x = fx;
		y = fy;
		z = fz;
		w = fw;
	};
	float4(Point4 data)
	{
		x = data.x;
		y = data.y;
		z = data.z;
		w = data.w;
	};
	float& operator [](int i)
	{
		return v[i];
	}
	void Normalize()
	{
		float re = 1 / (x + y + z + w);
		x *= re;
		y *= re;
		z *= re;
		w *= re;
	};
};

struct BonePoint
{
	int			m_NumWeight;
	MCHAR*		m_ID[4];
	float4		m_Weight;
};

struct BoneData
{
	float weigth;
	MCHAR* id;
};

struct MVERTEX
{
	float3 p;
	float3 t;
	float3 n;
	float3 tv;
};

struct CVERTEX
{
	float3	p;
	float3	t;
	float3	n;
	float3	tv;
	BonePoint bp;
};

struct int3
{
	int x, y, z;
	int3() {};
	int3(int fx, int fy, int fz)
	{
		x = fx;
		y = fy;
		z = fz;
	};
};

struct FaceIndex
{
	int3 m_Face[3];
};