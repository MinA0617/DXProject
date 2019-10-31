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
	float	PositionX	: packoffset(c0.x);
	float	PositionY	: packoffset(c0.y);
	float	ScaleX		: packoffset(c0.z);
	float	ScaleY		: packoffset(c0.w);
	float	Rotation	: packoffset(c1.x);
	float	b1empty1		: packoffset(c1.y);
	float	b1empty2		: packoffset(c1.z);
	float	b1empty3		: packoffset(c1.w);
};

struct VS2D_INPUT
{
	float3 Position			: POSITION;	 // simentci
	float2 UVPos			: TEXCOORD;
};

struct VS2D_OUTPUT // 444 please
{
	float4 Position			: SV_POSITION;	// reservation spell
	float2 UVPos			: TEXCOORD0;	// reservation spell
};

VS2D_OUTPUT VS( VS2D_INPUT input)
{
	VS2D_OUTPUT Output;
	Output.Position.x = input.Position.x * cos(Rotation) - input.Position.y * sin(Rotation);
	Output.Position.y = input.Position.x * sin(Rotation) + input.Position.y * cos(Rotation);
	Output.Position.x *= ScaleX;
	Output.Position.y *= ScaleY;
	Output.Position.x += PositionX;
	Output.Position.y += PositionY;
	Output.Position.x /= width/2;
	Output.Position.y /= height/2; 
	Output.Position.z = input.Position.z;
	Output.Position.w = 1.0;

	Output.UVPos = input.UVPos;

	return Output;
}