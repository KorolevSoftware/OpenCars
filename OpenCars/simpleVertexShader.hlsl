cbuffer ConstantBuffer : register(b0)

{

	matrix MatWorld; // Тут могут быть любые данные, понимаете? Не обязательно матрицы.

	matrix MatView;

	matrix MatProjection;

}



float4 VS(float4 Pos : POSITION) : SV_POSITION
{
	// Оставляем координаты точки без изменений
	return Pos;
}