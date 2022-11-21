// �}�e���A��
cbuffer ConstBufferDataMaterial : register(b0)
{
	float4 color_;	// �F(RGBA)
};

//�@3D�ϊ��s��
cbuffer ConstBufferDataTransform : register(b1)
{
	matrix mat_;	// 3D�ϊ��s��
};

// ���_�V�F�[�_�̏o�͍\����
// (���_�V�F�[�_����s�N�Z���V�F�[�_�ւ̂����Ɏg�p����)
struct VSOutput
{
	// �V�X�e���p���_���W
	float4 svpos_ : SV_POSITION;
	// �@���x�N�g��
	float3 normal_ : NORMAL;
	// uv�l
	float2 uv_ : TEXCOORD;
};