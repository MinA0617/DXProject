//-----------------------------------------------------------------------------
// Global variables
//-----------------------------------------------------------------------------
struct VS3D_INPUT
{
	float2	Position				: POSITION;	// reservation spell
	int		DivisionCount			: COUNT;
	int		NeighborType			: TYPE;
};

struct VS3D_OUTPUT
{
	float2	Position				: POSITION;	// reservation spell
	int		DivisionCount			: COUNT;
	int		NeighborType			: TYPE;
};

VS3D_OUTPUT VS( VS3D_INPUT input)
{
	return (VS3D_OUTPUT)input;
}