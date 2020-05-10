#pragma once


class ModelShader
{
public:

	//
	bool Init();

	//
	void Release();

	// 描画前準備
	//void SetUp();
	void SetUp();

	//===========================
	// 描画関数
	//===========================
	void DrawModel(const RgModel& model);


	//===========================
	// 設定関係
	//===========================
	void SetWorld(const RgMatrix& m) {
		m_cb0_Object.GetWork().mWorld = m;
	}


private:

	// シェーダオブジェクト
	ComPtr<ID3DBlob>  m_vs, m_ps;
	// 
	ComPtr<ID3D12RootSignature> m_rootSignature;
	ComPtr<ID3D12PipelineState> m_pipeline;

	// 定数バッファ
	struct cbObject
	{
		RgMatrix mWorld;
	};
	RgDynamicConstantBuffer<cbObject> m_cb0_Object;

	struct cbMaterial
	{
		RgVec4 color	= { 1, 1, 1, 1 };
		float roughness = 1.0f;
		float metallic = 0.0f;
	};
	RgDynamicConstantBuffer<cbMaterial> m_cb1_Material;
};