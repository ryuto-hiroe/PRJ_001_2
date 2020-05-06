
//============================
// �萔�o�b�t�@
//============================
cbuffer cbCamera : register(b10)
{
    row_major float4x4 g_mView;
    row_major float4x4 g_mProj;
}
cbuffer cbObject : register(b0)
{
    row_major float4x4 g_mWorld;
}
cbuffer cbMaterial : register(b1)
{
    float4 g_color;
    float g_roughness;
    float g_metaric;
}

//============================
// �e�V�F�[�_�̓��͍\����
//============================
struct VS_INPUT
{
    float4 pos : POSITION;  // ���W
    float3 normal : NORMAL; // �@��
    float2 uv : TEXCOORD0;  // UV���W
};
struct PS_INPUT
{
    float4 pos : SV_Position;   // �ˉe���W
    float3 wN : TEXCOORD0;      // ���[���h�@��
    float2 uv : TEXCOORD1;      // UV���W
};