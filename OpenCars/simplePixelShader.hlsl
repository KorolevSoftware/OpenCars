struct PS_INPUT
{
	float4 Pos : SV_POSITION;
	float4 Color : COLOR0;
	float4 posWorld : COLOR1;
};

float checkerboard(float2 pos, float scale) {
	float s = pos.x + 0.0001;
	float t = pos.y + 0.0001;

	float sum = floor(s * scale) + floor(t * scale);
	bool isEven = fmod(sum, 2.0) == 0.0;
	float percent = (isEven) ? 1.0 : 0.0;

	return percent;
}

float4 PS(PS_INPUT input) : SV_Target
{
	float ch = checkerboard(input.posWorld.xz, 2.0f);
	return  float4(input.Color.rgb *ch , 1.0f);
}