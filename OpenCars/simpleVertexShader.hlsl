cbuffer ConstantBuffer : register(b0)
{
	matrix MatWorld; // Тут могут быть любые данные, понимаете? Не обязательно матрицы.
}



struct VS_INPUT
{
	float4 Pos : POSITION;
	float4 Color : COLOR;
};

struct PS_INPUT
{
	float4 Pos : SV_POSITION;
	float4 Color : COLOR;
};


PS_INPUT VS(VS_INPUT input)
{
	// Оставляем координаты точки без изменений
	PS_INPUT output = (PS_INPUT)0;
	output.Pos = mul(input.Pos, MatWorld);
	output.Color = input.Color;
	return output;
}