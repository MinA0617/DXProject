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
	float4x4	m_matWorld		: packoffset(c0);
	float4  m_ObjectColor	: packoffset(c4);
};

cbuffer cb3 : register(b3)
{
	float4x4	m_BoneMat[256];
};

cbuffer cb4 : register(b4)
{
	float4x4	m_ZeroBoneMat[256];
};

float4x4 FetchBoneTransform(uint iBone)
{
	float4x4 mret;
	mret = m_BoneMat[iBone];
	return mret;
}

struct VS3D_INPUT
{
	float3	Position				: POSITION;	 // simentci
	float3	UVPos					: TEXCOORD;
	float3	Normal					: NORMAL;
	float3	Tangent					: TANGENTVECTOR;
	int		NumWeight				: NUMWEIGHT;
	int4	ID						: BONEID;
	float4	Weight					: BONEWEIGHT;
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
	VS3D_OUTPUT Output = (VS3D_OUTPUT)0;
	float4 Pos = float4(input.Position, 1);
	float3 Norm	= input.Normal;
	// ------------------------------
	for (int i = 0; i < input.NumWeight; i++)
	{
		float4x4 ZeroMat = m_ZeroBoneMat[input.ID[i]];
		float4x4 BoneMat = FetchBoneTransform(input.ID[i]);
		Output.Position += input.Weight[i] * mul(mul(Pos, ZeroMat), BoneMat);
		Output.Normal += input.Weight[i] * mul(Norm, (float3x3)BoneMat);
	}
	float3 Incident = normalize(Output.Position.xyz - g_EyePos);
	Output.Reflect = normalize(reflect(Incident, input.Normal));
	Output.WorldPosition = Output.Position.xyz;

	Output.Position = mul(Output.Position, g_matView);
	Output.Position = mul(Output.Position, g_matProj);

	Output.UVPos = input.UVPos;

	float3 Normal = normalize(Output.Normal);
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