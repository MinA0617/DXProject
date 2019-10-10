//-----------------------------------------------------------------------------
// Global variables
//-----------------------------------------------------------------------------

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
	matrix	m_matWorld		: packoffset(c0);
};

struct VSSPLINE_INPUT
{
	float3 Position			: POSITION;	 // simentci
	float3 Normal			: NORMAL;
	uint Type				: TYPE;
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
	Pos = mul(Pos, g_matView);
	Pos = mul(Pos, g_matProj);

	Output.Position = Pos;

	return Output;
}