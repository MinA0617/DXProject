//-----------------------------------------------------------------------------
// Global variables
//-----------------------------------------------------------------------------
#define g_iNumLight  1

Texture2D g_txDiffuse : register(t0);
Texture2D g_txNormal : register(t1);
TextureCube g_txEniv : register(t2);
Texture2D g_txSpecular : register(t3);
Texture2D g_txOpacity : register(t4);

SamplerState s0 : register(s0);

cbuffer cb0 : register(b0) // constant buffer name : register (empty : num)
{
	matrix	g_matViewProj;
	float3	g_EyePos;
	float	width;
	float3	g_EyeDir;
	float	height;
};

cbuffer cb1 : register(b1) // constant buffer name : register (empty : num)
{
	float3	m_ObjectColor;
	int		m_isDiffuse;
	int		m_isNormal;
	int		m_isSpecular;
	int		m_isOpacity;
	int		m_isReflection;
	int		m_isRefraction;
	int		m_isEnvironment;
};

cbuffer cb2 : register(b2) // constant buffer name : register (empty : num)
{
	matrix  g_InvMatrix :	packoffset(c0);
	float4	g_Lightpos :	packoffset(c4); // ���� ��ǥxyz �Ÿ�w
	float4	g_LightDir :	packoffset(c5); // ���� xyz, ����w
	float3	g_LightColor :	packoffset(c6);
	float	g_fInner :		packoffset(c6.w);	// ������ ������
	float	g_fOutner :		packoffset(c7.x);	// ������ �ܺ���
	float	g_fOffset :		packoffset(c7.y); // ���� �Ÿ��� ���� ���迡���� ������
	int		g_iFlag :		packoffset(c7.z);
	float	temp123 :		packoffset(c7.w);
};

struct VS3D_INPUT
{
	float3 Position				: POSITION;	 // simentci
	float3 UVPos				: TEXCOORD;
	float3 Normal				: NORMAL;
	float3 Tangent[g_iNumLight]	: TANGENTVECTOR;
	column_major matrix Worldmat				: WORLDMAT;
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
	Pos = mul(Pos, input.Worldmat);

	float3 Incident = normalize(Pos.xyz - g_EyePos);
	Output.Reflect = normalize(reflect(Incident, input.Normal));
	Output.WorldPosition = Pos.xyz;

	Pos = mul(Pos, g_matViewProj);

	Output.Position = Pos;
	Output.UVPos = input.UVPos;
	// ------- Output.Normal -------
	matrix m_matRotation = input.Worldmat;
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

struct PS3D_INPUT // 444 please
{
	float4 Position				: SV_POSITION;
	float3 UVPos				: TEXCOORD0;
	float3 Normal				: NORMAL;
	float3 WorldPosition		: POSITION0;
	float3 Reflect				: TEXCOORD1;
	float3 vLight[g_iNumLight]	: TEXCOORD2;
};

float3 ComputeLight(float3 Pos, float3 Normal)
{
	float3 LastColor = float3(0, 0, 0); // ����� �ʱ�ȭ
	switch (g_iFlag)
	{
	case 0: // SPOT
	{
		float4 LightVector;
		LightVector.w = distance(Pos, g_Lightpos.xyz); // �Ÿ�üũ
		if (LightVector.w > g_Lightpos.w) // �Ÿ�üũ
		{
			break;
		}
		else if (LightVector.w > g_fOffset)
		{
			float fLuminance = smoothstep(g_Lightpos.w, g_Lightpos.w - g_fOffset, LightVector.w);
			LightVector.xyz = normalize(Pos.xyz - g_Lightpos.xyz); // ����Ʈ���� ���� ���ϱ�
			float fDot = dot(Normal, -LightVector.xyz);
			LastColor.x += fDot * g_LightColor.x * fLuminance;
			LastColor.y += fDot * g_LightColor.y * fLuminance;
			LastColor.z += fDot * g_LightColor.z * fLuminance;
		}
		else
		{
			LightVector.xyz = normalize(Pos.xyz - g_Lightpos.xyz); // ����Ʈ���� ���� ���ϱ�
			float fDot = dot(Normal, -LightVector.xyz);
			LastColor.x += fDot * g_LightColor.x;
			LastColor.y += fDot * g_LightColor.y;
			LastColor.z += fDot * g_LightColor.z;
		}
		break;
	}
	case 1: // SKY
	{
		float fDot = dot(Normal, -g_LightDir.xyz);
		LastColor.x += fDot * g_LightColor.x;
		LastColor.y += fDot * g_LightColor.y;
		LastColor.z += fDot * g_LightColor.z;
		break;
	}
	case 2: // DIRECT
	{
		break;
	}
	case 3: // OMNI
	{
		float3 vLight = normalize(Pos - g_Lightpos.xyz);
		float fDistance = distance(Pos, g_Lightpos.xyz);
		float fLuminance = smoothstep(fDistance - g_fOffset, fDistance, g_Lightpos.w);
		float3 normal = Normal;
		float fIntensity = saturate(dot(Normal, -vLight));
		LastColor += float3(g_LightColor.rgb *  fLuminance * fIntensity);
		break;
	}
	}
	LastColor = max(LastColor, float3(0, 0, 0));
	return LastColor;
}
float ComputeSpecular(float Intensity, float3 Normal, float3 Pos)
{
	float result;
	for (int i = 0; i < g_iNumLight; i++)
	{
		switch (g_iFlag)
		{
		case 0:	// SPOT
		{
			break;
		}
		case 1:	// SKY
		{
			float3	R = reflect(g_LightDir.xyz, Normal);
			result = saturate(dot(R, -g_EyeDir)).x;
			break;
		}
		case 2:	// DIRECT
		{
			break;
		}
		case 3:	// OMNI
		{
			float3 vLight = normalize(Pos - g_Lightpos.xyz);
			float3 R = reflect(vLight, Normal);
			result = saturate(dot(R, -g_EyeDir)).x;
			break;
		}
		}
	}
	return result * Intensity;


}

float NormalMapping(float3 LightVector, float3 NormalMapPixel)
{
	NormalMapPixel *= 2;
	NormalMapPixel -= 1;

	float fDot = dot(LightVector, NormalMapPixel);
	return fDot;
}

float4 PS(PS3D_INPUT data) : SV_TARGET
{
	float4 lastcolor;
	if (m_isDiffuse == -1)
	{
		float4 Diffusecolor = g_txDiffuse.Sample(s0, data.UVPos.xy) * max(float4(0.1f, 0.1f, 0.1f, 1.0f), float4(ComputeLight(data.WorldPosition, data.Normal), 1));
		lastcolor = Diffusecolor;
	}
	else
	{
		float4 mutiplier = float4(0.3f, 0.3f, 0.3f, 1.0f) + float4(ComputeLight(data.WorldPosition, data.Normal), 1);
		lastcolor.xyz = m_ObjectColor * mutiplier.xyz;
	}
	if (m_isNormal == -1)
	{
		float4 Nomarlcolor = g_txNormal.Sample(s0, data.UVPos.xy);
		float normal = 0.0;
		for (int i = 0; i < g_iNumLight; i++)
		{
			normal += NormalMapping(data.vLight[i], Nomarlcolor.xyz);
		}
		lastcolor *= normal;
	}
	if (m_isSpecular != 0)
	{
		if (m_isSpecular == -1)
		{
			float4 SpecularColor = g_txSpecular.Sample(s0, data.UVPos.xy);
			float Specular = ComputeSpecular(SpecularColor.x, data.Normal, data.WorldPosition);
			lastcolor += Specular;
		}
		else
		{
			float Specular = ComputeSpecular(m_isSpecular / 255, data.Normal, data.WorldPosition);
			lastcolor += Specular;
		}
	}
	if (m_isEnvironment == -1)
	{
		float4 Enivcolor = g_txEniv.Sample(s0, data.Reflect);
		lastcolor = lerp(Enivcolor, lastcolor, 0.5);
	}
	if (m_isOpacity == -1)
	{
		float4 Opactiy = g_txOpacity.Sample(s0, data.UVPos.xy);
		lastcolor.w = Opactiy.x;
	}
	else
	{
		lastcolor.w = m_isOpacity / 255.00;
	}
	if (m_isReflection == -1)
	{
		//lastcolor.r = 1;
	}
	if (m_isRefraction == -1)
	{
		//lastcolor.g = 1;
	}
	if (m_isEnvironment == -1)
	{
		//lastcolor.b = 1;
	}
	return lastcolor;
}