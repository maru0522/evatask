// マテリアル
cbuffer ConstBufferDataMaterial : register(b0)
{
	float4 color_;	// 色(RGBA)
};

//　3D変換行列
cbuffer ConstBufferDataTransform : register(b1)
{
	matrix mat_;	// 3D変換行列
};

// 頂点シェーダの出力構造体
// (頂点シェーダからピクセルシェーダへのやり取りに使用する)
struct VSOutput
{
	// システム用頂点座標
	float4 svpos_ : SV_POSITION;
	// 法線ベクトル
	float3 normal_ : NORMAL;
	// uv値
	float2 uv_ : TEXCOORD;
};