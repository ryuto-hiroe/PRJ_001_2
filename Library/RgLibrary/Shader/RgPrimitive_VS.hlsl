
#include "inc_RgPrimitive.hlsli"


PS_INPUT main(VS_INPUT In)
{
    PS_INPUT result;
    
    // ���W�ϊ�
    result.pos = mul(In.pos, g_mWorld);
    result.pos = mul(result.pos, g_mView);
    result.pos = mul(result.pos, g_mProj);
    
    return result;
}