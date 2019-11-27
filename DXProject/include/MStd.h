#pragma once
#define _CRT_SECURE_NO_WARNINGS
#define MAX_BONE 64
#include <windows.h>

#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dx11.lib")
#pragma comment (lib, "dxgi.lib")

#pragma comment( lib, "msimg32")
#pragma comment (lib, "winmm.lib")

#pragma comment( lib, "d2d1.lib")
#pragma comment( lib, "dwrite.lib")

#if defined(_DEBUG)
#pragma comment (lib, "d3dx10d.lib")
#else
#pragma comment (lib, "d3dx10.lib")
#endif

#include <d3d11.h>
#include <d3dx11.h>
#include <d3d11.h>
#include <d2d1.h>
#include <d2d1helper.h>
#include <DWrite.h>
#include <D3DX10math.h>

#include <list>
#include <map>
#include <tchar.h>
#include <vector>
#include <set>
#include <string>
#include <algorithm>
#include <functional>
#include <assert.h>
#include <memory>
#include <iostream>
#include <fstream>
#include <sstream>
#include "time.h"

using namespace std;

#define TCORE_START int APIENTRY wWinMain(_In_ HINSTANCE hInstance,_In_opt_ HINSTANCE hPrevInstance,_In_ LPWSTR    lpCmdLine,_In_ int       nCmdShow){
#define TCORE_MAIN(s,x,y,w,h)  MSample  sample;sample.InitWindow(hInstance,L#s,x,y,w,h);sample.Run();
//#define TCORE_END _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);}
#define TCORE_END }
#define TCORE_RUN(s,x,y,w,h) TCORE_START; TCORE_MAIN(s,x,y,w,h); TCORE_END;

#define WM_UICONTRLMSG (WM_USER+1)

#define MFOR(x) for(int i=0; i<x; i++) 
#define BOOL_CHANGE(temp) temp = !temp

#define SAFE_RELEASE(temp)	if (temp){temp->Release();temp = NULL;}
#define SAFE_DELETE(temp)	if(temp){delete temp;}

using M_STR = basic_string<TCHAR>;

//using M_STR = wstring;
//struct D3DXVECTOR2;
//struct D3DXVECTOR3;
//
//struct D3DXVECTOR2
//{
//	union
//	{
//		struct { float x, y; };
//		float v[2];
//	};
//	D3DXVECTOR2() {};
//	D3DXVECTOR2(float fx, float fy)
//	{
//		x = fx;
//		y = fy;
//	};
//};
//struct D3DXVECTOR3
//{
//	union
//	{
//		struct { float x, y, z; };
//		float v[3];
//	};
//	D3DXVECTOR3 operator - (D3DXVECTOR3 data)
//	{
//		D3DXVECTOR3 result;
//		result.x = x - data.x;
//		result.y = y - data.y;
//		result.z = z - data.z;
//		return result;
//	};
//	D3DXVECTOR3 operator + (D3DXVECTOR3 data)
//	{
//		D3DXVECTOR3 result;
//		result.x = x + data.x;
//		result.y = y + data.y;
//		result.z = z + data.z;
//		return result;
//	};
//	void operator += (D3DXVECTOR3 data)
//	{
//		x += data.x;
//		y += data.y;
//		z += data.z;
//	};
//	void operator /= (float num)
//	{
//		x /= num;
//		y /= num;
//		z /= num;
//	};
//	bool operator == (D3DXVECTOR3 data)
//	{
//		if (x == data.x && y == data.y && z == data.z)
//		{
//			return true;
//		}
//		return false;
//	};
//	bool operator != (D3DXVECTOR3 data)
//	{
//		if (x == data.x && y == data.y && z == data.z)
//		{
//			return false;
//		}
//		return true;
//	};
//	D3DXVECTOR3() {};
//	D3DXVECTOR3(float fx, float fy, float fz)
//	{
//		x = fx;
//		y = fy;
//		z = fz;
//	};
//	D3DXVECTOR3(D3DXVECTOR3 data)
//	{
//		x = data.x;
//		y = data.y;
//		z = data.z;
//	};
//};
//struct D3DXVECTOR4
//{
//	union
//	{
//		struct { float x, y, z, w; };
//		float v[4];
//	};
//	D3DXVECTOR4() {};
//	D3DXVECTOR4(float fx, float fy, float fz, float fw)
//	{
//		x = fx;
//		y = fy;
//		z = fz;
//		w = fw;
//	};
//};

struct SVERTEX
{
	D3DXVECTOR3 p;
	D3DXVECTOR2 t;
	SVERTEX() {};
	SVERTEX(float px, float py, float pz, float tu, float tv)
	{
		p.x = px;
		p.y = py;
		p.z = pz;
		t.x = tu;
		t.y = tv;
	};
};

struct MVERTEX
{
	D3DXVECTOR3	p;
	D3DXVECTOR3	t;
	D3DXVECTOR3	n;
	D3DXVECTOR3	tv;
};

struct SKINDATA
{
	int		m_NumWeight;
	int		m_ID[4];
	float	m_Weight[4];
};

struct BonePoint
{
	int			m_NumWeight;
	M_STR		m_ID[4];
	float		m_Weight[4];
};

struct CVERTEX
{
	D3DXVECTOR3	p;
	D3DXVECTOR3	t;
	D3DXVECTOR3	n;
	D3DXVECTOR3	tv;
	BonePoint bp;
};


struct SPLINE_VERTEX
{
	D3DXVECTOR3 p;
};

//struct OBJIndex
//{
//	DWORD v;
//	DWORD vt;
//	DWORD vn;
//};
//
//struct OBJFace
//{
//	OBJIndex v[3];
//};

struct MSize
{
	int w;
	int h;
	MSize() {};
	MSize(int fw, int fh) { w = fw; h = fh; }
};

//struct D3DXVECTOR2
//{
//	float  x;
//	float  y;
//	D3DXVECTOR2 operator+ (D3DXVECTOR2 data) { D3DXVECTOR2 temp; temp.x = x + data.x; temp.y = y + data.y; };
//	void operator+= (D3DXVECTOR2 data) { x += data.x; y += data.y; };
//	D3DXVECTOR2() {};
//	D3DXVECTOR2(float fx, float fy) { x = fx; y = fy; }
//};

//struct M3Point
//{
//	float  x;
//	float  y;
//	float  z;
//	M3Point() {};
//	M3Point(float fx, float fy, float fz) { x = fx; y = fy; z = fz; }
//};

struct VS_WORLD_BUFFER
{
	D3DXMATRIX matViewProj;
	D3DXVECTOR3 vEyePos;
	float fClinetSizeW;		//c0.x
	D3DXVECTOR3 vEyeDir;
	float fClinetSizeH;		//c0.y
};

struct CONSTANT_2DOBJ
{
	D3DXVECTOR2	ScreenPosition;
	D3DXVECTOR2	ScreenScale;
	float	ScreenRotation;
	float	empty1, empty2, empty3;
};

struct CONSTANT_3DOBJ
{
	D3DXMATRIX		matWorld;
};

struct CONSTANT_BONE
{
	D3DXMATRIX		matBoneWorld;
};

struct CONSTANT_Matrial
{
	D3DXVECTOR3		ObjectColor;
	//bool			isDiffuse;
	//bool			isNormal;
	//bool			isSpecular;
	//bool			isOpacity;
	//bool			isReflection;
	//bool			isRefraction;
	//bool			isEnvironment;
	int				isDiffuse;
	int				isNormal;
	int				isSpecular;
	int				isOpacity;
	int				isReflection;
	int				isRefraction;
	int				isEnvironment;
	int				empty1;
	int				empty2;
};

enum { KEY_FREE = 0, KEY_PUSH = 1, KEY_HOLD = 2, KEY_UP = 3 };

struct Keyinput
{
	int   a_LeftClick;
	int   a_RightClick;

	int F1;
	int F2;
	int F3;
	int F4;
	int F5;
	int F6;
	int F7;
	int F8;
	int F9;
	int F10;
	int F11;
	int F12;

	int Num1;
	int Num2;
	int Num3;
	int Num4;
	int Num5;
	int Num6;

	int Insert;
	int Home;
	int PgUp;
	int Delete;
	int End;
	int PgDn;

	int Q;
	int W;
	int E;
	int R;
	int T;
	int Y;

	int A;
	int S;
	int D;
	int F;
	int G;
	int H;

	int Z;
	int X;
	int C;
	int V;
	int B;
	int N;

	int UP;
	int DOWN;
	int LEFT;
	int RIGHT;
	int SPACE;
	int ENTER;

	int CTRL;
	int ALT;
	int SHIFT;
};

template <typename T>
class MSingleton
{
public:
	MSingleton()
	{

	}
	~MSingleton()
	{

	}
	static T& GetInstance()
	{
		static T singleton;
		return singleton;
	}
};



extern Keyinput					g_ActionInput; // 액션 키 맵
extern HWND						g_hWnd; // 윈도우 핸들
extern int						g_iFramePerSecond ;
extern float					g_fSeoundPerFrame; // 프레임 당 시간
extern HINSTANCE				g_hInstance; // 윈도우 인스턴스
extern RECT						g_rtWindowClient;
extern ID3D11Device*			g_pDevice;
extern ID3D11DeviceContext*		g_pImmediateContext;
extern ID3D11RenderTargetView*	g_pRenderTargetView;
extern ID3D11DepthStencilView*	g_pDepthStencilView;
extern D3D11_VIEWPORT			g_ViewPort;
extern D3DXVECTOR2				g_MousePos; // 마우스 포지션

extern float		g_BGMVol;
extern float		g_SEVol;

extern bool							g_bIsLOD;
#if defined(DEBUG) || defined(_DEBUG)
extern bool							g_isBoxRender;
extern bool							g_isPointRender;
#endif