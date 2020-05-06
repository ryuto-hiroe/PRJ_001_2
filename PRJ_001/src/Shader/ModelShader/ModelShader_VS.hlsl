
// ���[�g�V�O�l�`����`


#include "ModelShader.hlsli"

PS_INPUT main(VS_INPUT In)
{
    PS_INPUT result;
    
    // ���W�ϊ�
    result.pos = mul(In.pos, g_mWorld);
    result.pos = mul(result.pos, g_mView);
    result.pos = mul(result.pos, g_mProj);
    
    // �@�������[���h�ϊ�
    result.wN = normalize(mul(In.normal, (float3x3) g_mWorld));
    
    // ���̂܂�UV��n��
    result.uv = In.uv;
    
    return result;
}