

//==============================
// 各シェーダ入力構造体
//==============================
// 頂点シェーダ
struct VS_INPUT
{
    float4 pos : POSITION; // 座標
};
// ピクセルシェーダ
struct PS_INPUT
{
    float4 pos : SV_Position; // 射影座標
};

//==============================
// 定数バッファ
//==============================
cbuffer cbObject : register(b0)
{
    row_major float4x4 g_mWorld;
    float4 g_color;
}

// カメラ
cbuffer cbCamera : register(b10)
{
    row_major float4x4 g_mView;
    row_major float4x4 g_mProj;
}