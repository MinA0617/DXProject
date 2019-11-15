//-----------------------------------------------------------------------------
// Global variables
//-----------------------------------------------------------------------------
#define g_iNumLight  1

cbuffer cb0 : register(b0) // constant buffer name : register (empty : num)
{
	matrix	g_matView;
	matrix	g_matProj;
	float3	g_EyePos;
	float	width;
	float3	g_EyeDir;
	float	height;
};

cbuffer cb1 : register(b1) // constant buffer name : register (empty : num)
{
	float3	m_ObjectColor;
	int		m_isAlpha;
	float2	m_WorldSize;
	int		m_isDiffuse[2];
	int		m_isNormal[2];
};

cbuffer cb2 : register(b2) // constant buffer name : register (empty : num)
{
	matrix  g_InvMatrix :	packoffset(c0);
	float4	g_Lightpos :	packoffset(c4); // ���� ��ǥxyz �Ÿ�w
	float4	g_LightDir :	packoffset(c5); // ���� xyz, ����w
	float3	g_LightColor :	packoffset(c6);
	float	g_fInner :		packoffset(c6.w);	// ������ ������
	float	g_fOutner :		packoffset(c7.x);	// ������ �ܺ���
	float	g_fOffset :		packoffset(c7.y); // ���� �Ÿ��� ���� ���迡���� ������
	int		g_iFlag :		packoffset(c7.z);
	float	temp123 :		packoffset(c7.w);
};

struct VS3D_INPUT
{
	float3 Position				: POSITION;	 // simentci
	float3 UVPos				: TEXCOORD;
	float3 Normal				: NORMAL;
	float3 Tangent				: TANGENTVECTOR;
};

struct VS3D_OUTPUT // 444 please
{
	float4 Position				: SV_POSITION;	// reservation spell
	float3 UVPos				: TEXCOORD0;	// reservation spell
	float3 Normal				: NORMAL;
	float3 WorldPosition		: POSITION0;
	float3 vLight[g_iNumLight]	: TEXCOORD2;
};

VS3D_OUTPUT VS( VS3D_INPUT input)
{
	VS3D_OUTPUT Output;
	float4 Pos = float4(input.Position, 1);

	Output.WorldPosition = Pos;

	Pos = mul(Pos, g_matView);
	Pos = mul(Pos, g_matProj);

	Output.Position = Pos;

	Output.UVPos = input.UVPos;
	// ------- Output.Normal -------
	Output.Normal = input.Normal;
	// -----------------------------

	float3 Normal = input.Normal;
	float3 Tangent = input.Tangent;
	float3 BiNormal = normalize(cross(Normal, Tangent));
	for (int i = 0; i < g_iNumLight; i++)
	{
		float3x3 mat =
		{
			Tangent.x,BiNormal.x,Normal.x,
			Tangent.y,BiNormal.y,Normal.y,
			Tangent.z,BiNormal.z,Normal.z
		};
		Output.vLight[i] = normalize(mul(-g_LightDir.xyz, mat));
	}

	return Output;
}