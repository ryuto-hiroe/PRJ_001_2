

//==============================
// �e�V�F�[�_���͍\����
//==============================
// ���_�V�F�[�_
struct VS_INPUT
{
    float4 pos : POSITION; // ���W
};
// �s�N�Z���V�F�[�_
struct PS_INPUT
{
    float4 pos : SV_Position; // �ˉe���W
};

//==============================
// �萔�o�b�t�@
//==============================
cbuffer cbObject : register(b0)
{
    row_major float4x4 g_mWorld;
    float4 g_color;
}

// �J����
cbuffer cbCamera : register(b10)
{
    row_major float4x4 g_mView;
    row_major float4x4 g_mProj;
}