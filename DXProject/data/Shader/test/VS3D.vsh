//-----------------------------------------------------------------------------
// Global variables
//-----------------------------------------------------------------------------

struct VS_INPUT
{
	float2	Position				: POSITION;	 // simentci
	float2	WorldPos				: WORLDPOS;
	float	Level					: LEVEL;
};

struct VS_OUTPUT // 444 please
{
	float2	Position				: POSITION;	 // simentci
	float2	WorldPos				: WORLDPOS;
	float	Level					: LEVEL;
};

VS_OUTPUT VS( VS_INPUT input)
{
	VS_OUTPUT Output;
	Output.Position = input.Position;
	Output.WorldPos = input.WorldPos;
	Output.Level = input.Level;
	return Output;
}
