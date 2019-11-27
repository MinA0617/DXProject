//-----------------------------------------------------------------------------
// Global variables
//-----------------------------------------------------------------------------
#define g_iNumLight  1
#define g_iMaxTileingTexture	20

SamplerState s0				: register(s0);
Texture2D g_txHeight		: register(t0);

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
	float g_FiledSize;
	float g_TileSize;
	float g_Heigth;
	float temp;
};

cbuffer cb2 : register(b2) // constant buffer name : register (empty : num)
{
	matrix  g_InvMatrix :	packoffset(c0);
	float4	g_Lightpos :	packoffset(c4); // 월드 좌표xyz 거리w
	float4	g_LightDir :	packoffset(c5); // 방향 xyz, 세기w
	float3	g_LightColor :	packoffset(c6);
	float	g_fInner : packoffset(c6.w);	// 스팟의 내부콘
	float	g_fOutner : packoffset(c7.x);	// 스팟의 외부콘
	float	g_fOffset : packoffset(c7.y); // 원의 거리에 따른 감쇠에서의 오프셋
	int		g_iFlag : packoffset(c7.z);
	float	temp123 : packoffset(c7.w);
};

struct GS3D_INPUT
{
	float2	Position				: POSITION;	// reservation spell
	int		DivisionCount			: COUNT;
	int		NeighborType			: TYPE;
};

struct GS3D_OUTPUT // 444 please
{
	float4	Position			: SV_POSITION;	// reservation spell
	float3	Normal				: NORMAL;
	float3	WorldPosition		: POSITION0;
	float3	vLight[g_iNumLight]	: TEXCOORD2;
};

GS3D_OUTPUT CreateMidVertex(GS3D_OUTPUT v0, GS3D_OUTPUT v1)
{
	GS3D_OUTPUT Mid = (GS3D_OUTPUT)0;
	Mid.WorldPosition = (v0.WorldPosition + v1.WorldPosition) / 2;
	float2 uv;
	uv.x = Mid.WorldPosition.x;
	uv.y = Mid.WorldPosition.z;
	float4 sampleresult = g_txHeight.Sample(s0, ((uv / g_FiledSize) + 0.5));
	Mid.WorldPosition.y = sampleresult.x;
	Mid.Normal.x = sampleresult.y * 2 - 1;
	Mid.Normal.y = sampleresult.z * 2 - 1;
	Mid.Normal.z = sampleresult.w * 2 - 1;

	float4 Pos = float4(Mid.WorldPosition, 1);
	Pos = mul(Pos, g_matView);
	Pos = mul(Pos, g_matProj);
	Mid.Position = Pos;

	for (int i = 0; i < g_iNumLight; i++)
	{
		Mid.vLight[i] = float3(0, 1, 0);
	}

	return Mid;
}

[maxvertexcount(4)]
void GS(in point GS3D_INPUT vIn[1], inout TriangleStream<GS3D_OUTPUT> vOut)
{
	int TileCount = pow(2, vIn[0].DivisionCount - 1);
	TileCount = max(TileCount, 64);
	GS3D_OUTPUT Output[64 * 64];
	//GS3D_OUTPUT Output[(TileCount + 1) * (TileCount + 1)];

	for (int i = 0; i < TileCount + 1; i++)
	{
		for (int j = 0; j < TileCount + 1; j++)
		{
			int flag = (i * (TileCount + 1)) + j;
			float2 worldpos;
			worldpos.x = vIn[0].Position.x - (g_TileSize / 2) + (g_TileSize * (i / TileCount));
			worldpos.y = vIn[0].Position.y - (g_TileSize / 2) + (g_TileSize * (j / TileCount));
			float4 sampleresult = g_txHeight.Sample(s0, ((worldpos / g_FiledSize) + 0.5));

			Output[flag].WorldPosition.x = worldpos.x;
			Output[flag].WorldPosition.z = worldpos.y;
			Output[flag].WorldPosition.y = sampleresult.z * g_Heigth;

			float4 Pos = float4(Output[flag].WorldPosition, 1);
			Pos = mul(Pos, g_matView);
			Pos = mul(Pos, g_matProj);

			Output[flag].Position = Pos;

			Output[flag].Normal.x = sampleresult.y * 2 - 1;
			Output[flag].Normal.y = sampleresult.z * 2 - 1;
			Output[flag].Normal.z = sampleresult.w * 2 - 1;

			for (int k = 0; k < g_iNumLight; k++)
			{
				Output[flag].vLight[k] = float3(0, 1, 0);
			}
		}
	}

	bool IsDifferent[4] = { 0,0,0,0 };
	int type = (int)vIn[0].NeighborType;
	if (type > 8) { type -= 8;		IsDifferent[3] = true; }
	if (type > 4) { type -= 4;		IsDifferent[2] = true; }
	if (type > 2) { type -= 2;		IsDifferent[1] = true; }
	if (type > 1) { type -= 1;		IsDifferent[0] = true; }

	for (int i = 0; i < TileCount; i++)
	{
		for (int j = 0; j < TileCount; j++)
		{
			int flag[4];
			flag[0] = i * (TileCount + 1) + j;
			flag[1] = flag[0] + 1;
			flag[2] = flag[1] + (TileCount + 1);
			flag[3] = flag[0] + (TileCount + 1);

			GS3D_OUTPUT Center = (GS3D_OUTPUT)0;
			for (int k = 0; k < 4; k++)
			{
				Center.WorldPosition += Output[flag[k]].WorldPosition;
			}
			Center.WorldPosition /= 4;
			float2 uv = float2(Center.WorldPosition.x, Center.WorldPosition.z);
			float4 sampleresult = g_txHeight.Sample(s0, ((uv / g_FiledSize) + 0.5));
			Center.WorldPosition.y = sampleresult.x;

			float4 Pos = float4(Center.WorldPosition, 1);
			Pos = mul(Pos, g_matView);
			Pos = mul(Pos, g_matProj);
			Center.Position = Pos;

			Center.Normal.x = sampleresult.y * 2 - 1;
			Center.Normal.y = sampleresult.z * 2 - 1;
			Center.Normal.z = sampleresult.w * 2 - 1;

			for (int k = 0; k < g_iNumLight; k++)
			{
				Center.vLight[k] = float3(0, 1, 0);
			}

			if (IsDifferent[0] && i == 0)
			{
				GS3D_OUTPUT Mid = CreateMidVertex(Output[flag[0]], Output[flag[1]]);
				vOut.Append(Output[flag[1]]);
				vOut.Append(Mid);
				vOut.Append(Center);
				vOut.Append(Output[flag[0]]);
				vOut.Append(Center);
				vOut.Append(Mid);
			}
			else
			{
				vOut.Append(Output[flag[1]]);
				vOut.Append(Output[flag[0]]);
				vOut.Append(Center);
			}
			if (IsDifferent[2] && i == TileCount - 1)
			{
				GS3D_OUTPUT Mid = CreateMidVertex(Output[flag[1]], Output[flag[2]]);
				vOut.Append(Output[flag[2]]);
				vOut.Append(Mid);
				vOut.Append(Center);
				vOut.Append(Output[flag[1]]);
				vOut.Append(Center);
				vOut.Append(Mid);
			}
			else
			{
				vOut.Append(Output[flag[2]]);
				vOut.Append(Output[flag[1]]);
				vOut.Append(Center);
			}
			if (IsDifferent[3] && j == 0)
			{
				GS3D_OUTPUT Mid = CreateMidVertex(Output[flag[2]], Output[flag[3]]);
				vOut.Append(Output[flag[3]]);
				vOut.Append(Mid);
				vOut.Append(Center);
				vOut.Append(Output[flag[2]]);
				vOut.Append(Center);
				vOut.Append(Mid);
			}
			else
			{
				vOut.Append(Output[flag[3]]);
				vOut.Append(Output[flag[2]]);
				vOut.Append(Center);
			}
			if (IsDifferent[1] && j == TileCount - 1)
			{
				GS3D_OUTPUT Mid = CreateMidVertex(Output[flag[0]], Output[flag[3]]);
				vOut.Append(Output[flag[0]]);
				vOut.Append(Mid);
				vOut.Append(Center);
				vOut.Append(Output[flag[3]]);
				vOut.Append(Center);
				vOut.Append(Mid);
			}
			else
			{
				vOut.Append(Output[flag[0]]);
				vOut.Append(Output[flag[3]]);
				vOut.Append(Center);
			}
		}
	}
	vOut.RestartStrip();
}