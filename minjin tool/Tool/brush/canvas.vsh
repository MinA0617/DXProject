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

struct VS2D_INPUT
{
	float4 Data			: DATA;	 // simentci
};

struct VS2D_OUTPUT // 444 please
{
	float4 Position			: SV_POSITION;	// reservation spell
	float2 UVPos			: TEXCOORD0;	// reservation spell
};

VS2D_OUTPUT VS(VS2D_INPUT input)
{
	VS2D_OUTPUT Output;
	//Output.Position.x = input.Data.x * cos(Rotation) - input.Data.y * sin(Rotation);
	//Output.Position.y = input.Data.x * sin(Rotation) + input.Data.y * cos(Rotation);
	Output.Position.x = input.Data.x;
	Output.Position.y = input.Data.y;
	Output.Position.x *= ScaleX;
	Output.Position.y *= ScaleY;
	Output.Position.x += PositionX;
	Output.Position.y += PositionY;
	Output.Position.x /= width / 2;
	Output.Position.y /= height / 2;
	Output.Position.z = 0;
	Output.Position.w = 1.0;

	Output.UVPos.x = input.Data.z;
	Output.UVPos.y = input.Data.w;

	return Output;
}

struct PS2D_INPUT // 444 please
{
	float4 Position			: SV_POSITION;	// register
	float2 UVPos			: TEXCOORD0;	// register
};

float4 PS(PS2D_INPUT data) : SV_TARGET
{
	float4 color = g_txDiffuse.Sample(s0, data.UVPos);
	float4 lastcolor = (float4)0;
	if (channel == 0) lastcolor.x = color.x * Rotation;
	if (channel == 1) lastcolor.y = color.x * Rotation;
	if (channel == 2) lastcolor.z = color.x * Rotation;
	lastcolor.w = color.w;
	return lastcolor;
}