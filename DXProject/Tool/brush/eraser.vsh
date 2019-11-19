Texture2D g_txDiffuse : register(t0);

SamplerState s0 : register(s0);

cbuffer cb0 : register(b0)
{
	float	PositionX : packoffset(c0.x);
	float	PositionY : packoffset(c0.y);
	float	ScaleX : packoffset(c0.z);
	float	ScaleY : packoffset(c0.w);
	float	Rotation : packoffset(c1.x);
	float	width : packoffset(c1.y);
	float	height : packoffset(c1.z);
	float	channel : packoffset(c1.w);
};

struct PS2D_INPUT // 444 please
{
	float4 Position			: SV_POSITION;	// register
	float2 UVPos			: TEXCOORD0;	// register
};

float4 PS(PS2D_INPUT data) : SV_TARGET
{
	float4 color = g_txDiffuse.Sample(s0, data.UVPos);
	float4 lastcolor = (float4)0;
	if (channel == 0) lastcolor.x = color.x;
	if (channel == 1) lastcolor.y = color.x;
	if (channel == 2) lastcolor.z = color.x;
	lastcolor.w = color.w * Rotation;
	return lastcolor;
}