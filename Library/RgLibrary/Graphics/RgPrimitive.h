#pragma once


class RgPrimitive
{
public:

	// 初期化
	bool Init();
	// 解放
	void Release();

	// 描画前準備
	void SetUp();


	// 頂点構造体
	//struct RgVertex
	//{
	//	RgVec3 pos;		// 頂点座標
	//	RgVec2 uv;		// UV座標
	//};

private:
	// シェーダオブジェクト
	ComPtr<ID3DBlob>  m_vs, m_ps;
	// 
	ComPtr<ID3D12RootSignature> m_rootSignature;
	ComPtr<ID3D12PipelineState> m_pipeline;

	//=======================
	// リソース
	//=======================
	// 頂点・インデックスバッファ
	ComPtr<ID3D12Resource1> m_vertexBuffer;
	ComPtr<ID3D12Resource1> m_indexBuffer;
	D3D12_VERTEX_BUFFER_VIEW  m_vertexBufferView;
	D3D12_INDEX_BUFFER_VIEW   m_indexBufferView;
	UINT  m_indexCount;
};