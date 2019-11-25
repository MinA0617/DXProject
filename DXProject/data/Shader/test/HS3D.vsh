//-----------------------------------------------------------------------------
// Global variables
//-----------------------------------------------------------------------------
#define g_iNumLight  1
#define MAX_POINTS 32
cbuffer cb0 : register(b0) // constant buffer name : register (empty : num)
{
	matrix	g_matView;
	matrix	g_matProj;
	float3	g_EyePos;
	float	width;
	float3	g_EyeDir;
	float	height;
};

cbuffer cb1 : register(b1)
{
	float m_Size;
};

cbuffer cb2 : register(b2) // constant buffer name : register (empty : num)
{
	matrix  g_InvMatrix :	packoffset(c0);
	float4	g_Lightpos :	packoffset(c4); // 월드 좌표xyz 거리w
	float4	g_LightDir :	packoffset(c5); // 방향 xyz, 세기w
	float3	g_LightColor :	packoffset(c6);
	float	g_fInner :		packoffset(c6.w);	// 스팟의 내부콘
	float	g_fOutner :		packoffset(c7.x);	// 스팟의 외부콘
	float	g_fOffset :		packoffset(c7.y); // 원의 거리에 따른 감쇠에서의 오프셋
	int		g_iFlag :		packoffset(c7.z);
	float	temp123 :		packoffset(c7.w);
};

struct HS_CONSTANT_OUTPUT
{
	float edges[4] : SV_TessFactor;
	float inside[2] : SV_InsideTessFactor;
};

struct HS_INPUT
{
	float2	Position				: POSITION;
	float2	WorldPos				: WORLDPOS;
	float	Level					: LEVEL;
};

struct HS_OUTPUT // 444 please
{
	float2 Position				: SV_POSITION;	// reservation spell
	float2 WorldPos				: WORLDPOS;
};

HS_CONSTANT_OUTPUT PatchConstantFunction(InputPatch<HS_INPUT, MAX_POINTS> inputPatch, uint patchId : SV_PrimitiveID)
{
	HS_CONSTANT_OUTPUT output;
	//output.edges[0] = tessellationAmount;
	//output.edges[1] = tessellationAmount;
	//output.edges[2] = tessellationAmount;
	//output.edges[3] = tessellationAmount;
	//output.inside[0] = tessellationAmount;
	//output.inside[1] = tessellationAmount;
	output.edges[0] = 4;
	output.edges[1] = 4;
	output.edges[2] = 4;
	output.edges[3] = 4;
	output.inside[0] = 4;
	output.inside[1] = 4;
	return output;
}

[domain("quad")]
[partitioning("integer")]
[outputtopology("triangle_cw")]
[outputcontrolpoints(4)]
[patchconstantfunc("PatchConstantFunction")]
[maxtessfactor(64.0f)]
HS_OUTPUT HS(InputPatch<HS_INPUT, MAX_POINTS> Patch, uint pointId : SV_OutputControlPointID, uint patchId : SV_PrimitiveID)
{
	HS_OUTPUT output;
	output.Position = patch[pointId].Position;
	output.WorldPos = patch[pointId].WorldPos;
	return output;
}