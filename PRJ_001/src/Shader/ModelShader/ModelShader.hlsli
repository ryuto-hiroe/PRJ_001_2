
//============================
// 定数バッファ
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
// 各シェーダの入力構造体
//============================
struct VS_INPUT
{
    float4 pos : POSITION;  // 座標
    float3 normal : NORMAL; // 法線
    float2 uv : TEXCOORD0;  // UV座標
};
struct PS_INPUT
{
    float4 pos : SV_Position;   // 射影座標
    float3 wN : TEXCOORD0;      // ワールド法線
    float2 uv : TEXCOORD1;      // UV座標
};