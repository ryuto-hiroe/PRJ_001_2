#pragma once


//===============================================================
//
// テクスチャクラス
//
//		・RgTextureLoader
//		・RgTexture2D
//		・RgTexture3D
//
//===============================================================


//===============================================================
//
// 2Dテクスチャのクラス
//
//===============================================================
class RgTexture2D
{
public:

	//================================================
	//
	// 設定関係
	//
	//================================================
	// コマンド作成
	void CreateCommandSetSRV(const ComPtr<ID3D12GraphicsCommandList>& command, const UINT paraNum)const;

	//================================================
	//
	// 読み込み、作成関数
	//
	//================================================

	// ファイルからテクスチャ読み込み
	bool LoadTexture(const std::string& filename);

	// メモリからテクスチャを読み込む
	bool LoadTextureFromMemory(const std::vector<char>& imageData);

	
	// RTV付テクスチャを生成
	bool CreateTexAndSRV(const int w, const int h, DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM);


	// テクスチャを使用できる形に変更する
	bool SetUpTexture(
		const DirectX::ScratchImage& image,
		const DirectX::TexMetadata& mate);

	//================================================
	//
	// 取得関係
	//
	//================================================
	// リソースを取得
	const ComPtr<ID3D12Resource1> GetResource() { return m_texture2D; }

	//
	void Release();

private:

	//================================================
	//
	// データ
	//
	//================================================
	// ファイル名(LoadTextureした場合のみ)
	std::string			m_filepath;

	// テクスチャリソース
	ComPtr<ID3D12Resource1>			m_texture2D = nullptr;

	//-------------------------
	// SRV
	//-------------------------
	// SRV作成用構造体
	D3D12_SHADER_RESOURCE_VIEW_DESC m_srvDesc = {};
	// ハンドル
	//	12では、ディスクリプタヒープにViewを作成し、使うときは
	//	ディスクリプタヒープに作成した場所のハンドルを使用する
	RgHeapHandle m_srv;

	//-------------------------
	// RTV
	//-------------------------
	D3D12_RENDER_TARGET_VIEW_DESC m_rtvDesc = {};
	// 
	RgHeapHandle m_rtv;
};


//===============================================================
//
// Render Targetのクラス
//	・RTテクスチャを作成して管理するクラス
//	・マルチレンダーターゲット対応
//
//===============================================================
class RgRenderTargets
{
public:

	//================================================
	//
	// 作成関数
	//
	//================================================
	// レンダーターゲット作成
	//	・マルチレンダーターゲット対応
	bool CreateRT(
		const int w,
		const int h,
		std::vector<DXGI_FORMAT> format = { DXGI_FORMAT_R8G8B8A8_UNORM }
	);

	// ディスクリプタヒープの作成
	bool CreateDescriptorHeap();

	//================================================
	//
	// 設定関係
	//
	//================================================
	// レンダーターゲットを設定する
	void SetRenderTargets(ComPtr<ID3D12GraphicsCommandList>& comList);

	// シェーダリソースビューを設定する
	void CreateCommandSetSRV(const UINT rtNum, const ComPtr<ID3D12GraphicsCommandList>& command, const UINT paraNum)const;

	// RTをクリアする
	//void ClearRT()

	//================================================
	//
	// 取得関係
	//
	//================================================

	// 
	void Release(){}

private:
	// 
	std::vector<std::shared_ptr<RgTexture2D>> m_rt;
	// RTV用ディスクリプタヒープハンドル
	std::vector<CD3DX12_CPU_DESCRIPTOR_HANDLE> m_rtvHandles;

	// マルチレンダーターゲットの枚数
	UINT m_multiRTCount = 1;

	// RTV用ディスクリプタヒープ
	ComPtr<ID3D12DescriptorHeap> m_heapRtv;
	//
	UINT m_rtvDescriptorSize = 0;
	// 使用している数
	UINT m_useDescriptorNum = 0;
};