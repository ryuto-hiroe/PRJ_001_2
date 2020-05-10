#pragma once

struct RgVertex_Pos
{
	RgVec3 pos;
};

class RgPrimitiveShader
{
public:

	//
	bool Init();

	//
	void Release();

	// 描画前準備
	void SetUp();

	// カメラ設定
	void SetCamera(const RgMatrix& mV, const RgMatrix& mP)
	{
		m_cb10_Camera.GetWork().mView = mV;
		m_cb10_Camera.GetWork().mProj = mP;
		m_cb10_Camera.WriteData();
		//m_cb10_Camera.CreateCommandSetCBV(RGD3D.m_commandList, 1);
	}

	//===========================
	// 描画関数
	//===========================
	// 線の描画
	void DrawLine(
		const RgVec3& pos1,
		const RgVec3& pos2,
		const RgVec4& color,
		const RgMatrix* mat = (RgMatrix*)&RgMatrix::Identity
	);

	void DrawLine(
		const RgLineModel& model, 
		const RgVec4& color,
		const RgMatrix* mat = (RgMatrix*)&RgMatrix::Identity);


private:

	// シェーダオブジェクト
	ComPtr<ID3DBlob>  m_vs, m_ps;
	// 
	ComPtr<ID3D12RootSignature> m_rootSignature;
	ComPtr<ID3D12PipelineState> m_pipeline;

	// 頂点バッファ
	RgDynamicVertexBuffer<RgVertex_Pos> m_vb;


	//=========================
	// 定数バッファ
	//=========================
	struct cbObject
	{
		RgMatrix mWorld;
		RgVec4 color;
	};
	RgDynamicConstantBuffer<cbObject> m_cb0_Object;

	struct cbCamera
	{
		RgMatrix mView;
		RgMatrix mProj;
	};
	RgStaticConstantBuffer<cbCamera> m_cb10_Camera;
};