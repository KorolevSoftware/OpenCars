Texture2D shaderTexture : register(t0);
SamplerState SampleType : register(s0);

cbuffer ConstantBuffer : register(b0)
{
	matrix MatWorld;
}

struct PS_INPUT
{
	float4 Pos : SV_POSITION;
	float2 tex : TEXCOORD0;
};

struct VS_INPUT
{
	float4 Pos : POSITION;
	float2 tex : TEXCOORD0;
};

float3 tri(float3 x)
{
	return 1.0 - abs(2.0*frac(x / 2.0) - 1.0);
}

float checkersTextureGrad(float3 p, float scale, float3 ddxx, float3 ddyy)
{
	float3 w = max(abs(ddxx), abs(ddyy)) + 0.001; // filter kernel
	p *= scale;
	p += float3(0.001, 0.001, 0.001);
	float3 i = (tri(p + 0.5*w) - tri(p - 0.5*w)) / w;    // analytical integral (box filter)
	return 0.5 - 0.5*i.x*i.y*i.z;
}

float xorTextureGradBox(float3 pos, float scale, float3 ddx, float3 ddy)
{
	float xor = 0.0;
	for (int i = 0; i < 10; i++)
	{
		xor += checkersTextureGrad(pos, scale, ddx, ddy);
		ddx *= 0.5;
		ddy *= 0.5;
		pos *= 0.5;
		xor *= 0.5;
	}
	return xor;
}

float checkersTexture(float3 p, float scale)
{
	float3 q = floor((p + float3(0.001, 0.001, 0.001))*scale);
	return abs(fmod(q.x + q.y + q.z, 2.0));
}

PS_INPUT VS(VS_INPUT input)
{
	PS_INPUT output = (PS_INPUT)0;
	output.Pos = mul(input.Pos, MatWorld);
	output.tex = input.tex;
	return output;
}

float4 PS(PS_INPUT input) : SV_Target
{
	float4 textureColor;
	// Sample the pixel color from the texture using the sampler at this texture coordinate location.
	textureColor = shaderTexture.Sample(SampleType, input.tex);
	return textureColor;
}