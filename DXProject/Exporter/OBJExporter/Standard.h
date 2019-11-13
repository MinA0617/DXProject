#pragma once

using M_STR = std::basic_string<TCHAR>;

struct D3DXVECTOR3
{
	union
	{
		struct { float x, y, z; };
		float v[3];
	};
	D3DXVECTOR3 operator - (D3DXVECTOR3 data)
	{
		D3DXVECTOR3 result;
		result.x = x - data.x;
		result.y = y - data.y;
		result.z = z - data.z;
		return result;
	};
	bool operator == (D3DXVECTOR3 data)
	{
		if (x == data.x && y == data.y && z == data.z)
		{
			return true;
		}
		return false;
	};
	bool operator != (D3DXVECTOR3 data)
	{
		if (x == data.x && y == data.y && z == data.z)
		{
			return false;
		}
		return true;
	};
	D3DXVECTOR3() {};
	D3DXVECTOR3(float fx, float fy, float fz)
	{
		x = fx;
		y = fy;
		z = fz;
	};
	D3DXVECTOR3(Point3 data)
	{
		x = data.x;
		y = data.y;
		z = data.z;
	};
};

struct D3DXVECTOR4
{
	union
	{
		struct { float x, y, z, w; };
		float v[4];
	};
	D3DXVECTOR4() {};
	D3DXVECTOR4(float fx, float fy, float fz, float fw)
	{
		x = fx;
		y = fy;
		z = fz;
		w = fw;
	};
	D3DXVECTOR4(Point4 data)
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
	D3DXVECTOR4		m_Weight;
};

struct BoneData
{
	float weigth;
	MCHAR* id;
};

struct MVERTEX
{
	D3DXVECTOR3 p;
	D3DXVECTOR3 t;
	D3DXVECTOR3 n;
	D3DXVECTOR3 tv;
};

struct CVERTEX
{
	D3DXVECTOR3	p;
	D3DXVECTOR3	t;
	D3DXVECTOR3	n;
	D3DXVECTOR3	tv;
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