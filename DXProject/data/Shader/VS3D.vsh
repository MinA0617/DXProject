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
	matrix  g_InvMatrix;
	float4	g_Lightpos; // 월드 좌표xyz 거리w
	float4	g_LightDir; // 방향 xyz, 세기w
	float4	g_LightColor;
	float	g_fInner;	// 스팟의 내부콘
	float	g_fOutner;	// 스팟의 외부콘
	float	g_fOffset; // 원의 거리에 따른 감쇠에서의 오프셋
	int		g_iFlag;
};

cbuffer cb2 : register(b2)
{
	matrix	m_matWorld		: packoffset(c0);
	float4  m_ObjectColor	: packoffset(c4);
};

struct VS3D_INPUT
{
	float3 Position				: POSITION;	 // simentci
	float3 UVPos				: TEXCOORD;
	float3 Normal				: NORMAL;
	float3 Tangent[g_iNumLight]	: TANGENTVECTOR;
};

struct VS3D_OUTPUT // 444 please
{
	float4 Position				: SV_POSITION;	// reservation spell
	float3 UVPos				: TEXCOORD0;	// reservation spell
	float3 Normal				: NORMAL;
	float3 WorldPosition		: POSITION0;
	float3 Reflect				: TEXCOORD1;
	float3 vLight[g_iNumLight]	: TEXCOORD2;
};

VS3D_OUTPUT VS( VS3D_INPUT input)
{
	VS3D_OUTPUT Output;
	float4 Pos = float4(input.Position, 1);
	Pos = mul(Pos, m_matWorld);

	float3 Incident = normalize(Pos.xyz - g_EyePos);
	Output.Reflect = normalize(reflect(Incident, input.Normal));
	Output.WorldPosition = Pos.xyz;

	Pos = mul(Pos, g_matView);
	Pos = mul(Pos, g_matProj);

	Output.Position = Pos;
	Output.UVPos = input.UVPos;
	// ------- Output.Normal -------
	matrix m_matRotation = m_matWorld;
	m_matRotation._41 = 0.0;
	m_matRotation._42 = 0.0;
	m_matRotation._43 = 0.0;
	float4 normal = mul(float4 (input.Normal, 1), m_matRotation);
	normal = normalize(normal);
	Output.Normal = normal.xyz;
	// -----------------------------

	float3 Normal = normalize(input.Normal);
	for (int i = 0; i < g_iNumLight; i++)
	{
		float3 Tangent = normalize(input.Tangent[i]);
		float3 BiNormal = normalize(cross(Normal, Tangent));
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