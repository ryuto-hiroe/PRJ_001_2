
// ルートシグネチャ定義


#include "ModelShader.hlsli"

PS_INPUT main(VS_INPUT In)
{
    PS_INPUT result;
    
    // 座標変換
    result.pos = mul(In.pos, g_mWorld);
    result.pos = mul(result.pos, g_mView);
    result.pos = mul(result.pos, g_mProj);
    
    // 法線をワールド変換
    result.wN = normalize(mul(In.normal, (float3x3) g_mWorld));
    
    // そのままUVを渡す
    result.uv = In.uv;
    
    return result;
}