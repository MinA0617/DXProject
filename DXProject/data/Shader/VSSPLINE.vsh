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
	float4  m_ObjectColor	: packoffset(c4);
};

cbuffer cb2 : register(b2) // constant buffer name : register (empty : num)
{
	matrix  g_InvMatrix :	packoffset(c0);
	float4	g_Lightpos :	packoffset(c4); // ���� ��ǥxyz �Ÿ�w
	float4	g_LightDir :	packoffset(c5); // ���� xyz, ����w
	float3	g_LightColor :	packoffset(c6);
	float	g_fInner : packoffset(c6.w);	// ������ ������
	float	g_fOutner : packoffset(c7.x);	// ������ �ܺ���
	float	g_fOffset : packoffset(c7.y); // ���� �Ÿ��� ���� ���迡���� ������
	int		g_iFlag : packoffset(c7.z);
	float	temp123 : packoffset(c7.w);
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