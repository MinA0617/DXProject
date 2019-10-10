//-----------------------------------------------------------------------------
// Global variables
//-----------------------------------------------------------------------------

cbuffer cb0 : register(b0) // constant buffer name : register (empty : num)
{
	matrix	g_matWorld	: packoffset(c0);
	matrix	g_matView	: packoffset(c4);
	matrix	g_matProj	: packoffset(c8);
	float	width		: packoffset(c12.x); // place in register
	float	height		: packoffset(c12.y);
	float	b0empty1	: packoffset(c12.z);
	float	b0empty2	: packoffset(c12.w);
};

cbuffer cb1 : register(b1)
{
	matrix	m_matWorld		: packoffset(c0);
	float4  m_ObjectColor	: packoffset(c4);
};

struct VS3D_INPUT
{
	float3 Position			: POSITION;	 // simentci
	float3 UVPos			: TEXCOORD;
	float3 Normal			: NORMAL;
};

struct VS3D_OUTPUT // 444 please
{
	float4 Position			: SV_POSITION;	// reservation spell
	float4 UVPos			: TEXCOORD0;	// reservation spell
	float4 Normal			: NORMAL0;
};

VS3D_OUTPUT VS( VS3D_INPUT input)
{
	VS3D_OUTPUT Output;
	Output.Position = float4(input.Position, 1);
	Output.Position = mul(Output.Position, m_matWorld);
	Output.Position = mul(Output.Position, g_matView);
	Output.Position = mul(Output.Position, g_matProj);

	Output.UVPos = float4(input.UVPos, 0);
	Output.Normal = float4(input.Normal, 0);

	return Output;
}