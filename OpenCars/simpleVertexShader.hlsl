cbuffer ConstantBuffer : register(b0)

{

	matrix MatWorld; // ��� ����� ���� ����� ������, ���������? �� ����������� �������.

	matrix MatView;

	matrix MatProjection;

}



float4 VS(float4 Pos : POSITION) : SV_POSITION
{
	// ��������� ���������� ����� ��� ���������
	return Pos;
}