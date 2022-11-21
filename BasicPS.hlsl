#include "Basic.hlsli"

//Texture2D<float4> tex : register(t0);
Texture2D<float4> tex : register(t0);

SamplerState smp : register(s0);

float4 main(VSOutput input) : SV_TARGET
{
	float4 texcolor = float4(tex.Sample(smp,input.uv_));

	float3 light = normalize(float3(1,-1,1));						// �E���� �����̃��C�g
	float diffuse = saturate(dot(-light, input.normal_));			// diffuse��[0,1]�͈̔͂�Clamp����
	float brightness = diffuse + 0.3f;								// �A���r�G���g����0.3�Ƃ��Čv�Z
	return float4(texcolor.rgb * brightness, texcolor.a) * color_;	// �P�x��RGB�ɑ�����ďo��
}