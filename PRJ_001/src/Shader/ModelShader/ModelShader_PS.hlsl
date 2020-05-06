
#include "ModelShader.hlsli"

// テクスチャ
Texture2D g_tex : register(t0);
// サンプラ
SamplerState g_smp : register(s0);

float4 main(PS_INPUT In) : SV_Target0
{
    float4 outColor = g_tex.Sample(g_smp, In.uv) * g_color;
    if (outColor.a < 0.1f)
    {
        discard;
    }
    outColor.a = 1.0f;
    return outColor;
}