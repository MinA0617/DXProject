//-----------------------------------------------------------------------------
// Global variables
//-----------------------------------------------------------------------------
cbuffer cb0 : register(b0) // constant buffer name : register (empty : num)
{
	matrix	g_matViewProj;
	float3	g_EyePos;
	float	width;
	float3	g_EyeDir;
	float	height;
};

cbuffer cb1 : register(b1)
{
	matrix	m_matWorld		: packoffset(c0);
};

struct VSSPLINE_INPUT
{
	float3 Position			: POSITION;	 // simentci
};

struct VSSPLINE_OUTPUT // 444 please
{
	float4 Position			: SV_POSITION;	// reservation spell
};

VSSPLINE_OUTPUT VS(VSSPLINE_INPUT input)
{
	VSSPLINE_OUTPUT Output;

	float4 Pos = float4(input.Position, 1);
	Pos = mul(Pos, m_matWorld);
	Pos = mul(Pos, g_matViewProj);

	Output.Position = Pos;

	return Output;
}