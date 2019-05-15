cbuffer ConstantBuffer : register(b0)
{
	matrix MatWorld;
}

struct VS_INPUT
{
	float4 Pos : POSITION;
	float4 Color : COLOR;
};

struct PS_INPUT
{
	float4 Pos : SV_POSITION;
	float4 Color : COLOR0;
	float4 posWorld: COLOR1;
};


PS_INPUT VS(VS_INPUT input)
{
	// Оставляем координаты точки без изменений
	PS_INPUT output = (PS_INPUT)0;
	output.Pos = mul(input.Pos, MatWorld);
	output.Color = input.Color;
	output.posWorld = input.Pos;
	return output;
}