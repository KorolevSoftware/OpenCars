cbuffer ConstantBuffer : register(b0)
{
	matrix MatWorld;
}

struct VS_INPUT
{
	float4 Pos : POSITION;
	float2 tex : TEXCOORD0;
};

struct PS_INPUT
{
	float4 Pos : SV_POSITION;
	float2 tex : TEXCOORD0;
};


PS_INPUT VS(VS_INPUT input)
{
	PS_INPUT output = (PS_INPUT)0;
	output.Pos = mul(input.Pos, MatWorld);
	output.tex = input.tex;
	return output;
}