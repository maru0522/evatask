#include "Basic.hlsli"

VSOutput main(float4 pos : POSITION,float3 normal : NORMAL, float2 uv : TEXCOORD)
{
	VSOutput output;	// ピクセルシェーダーに渡す値
	output.svpos_ = mul(mat_, pos);	// 座標に行列を乗算
	output.normal_ = normal;
	output.uv_ = uv;
	return output;
}