//-----------------------------------------------------------------------------
// Global variables
//-----------------------------------------------------------------------------

struct VS_INPUT
{
	float3	Position				: POSITION;	 // simentci
	float2	WorldPos				: WORLDPOS;
};

struct VS_OUTPUT // 444 please
{
	float3	Position				: POSITION;	 // simentci
	float2	WorldPos				: WORLDPOS;
};

VS_OUTPUT VS( VS_INPUT input)
{
	VS_OUTPUT Output;
	Output.Position = input.Position;
	Output.WorldPos = input.WorldPos;
	return Output;
}
