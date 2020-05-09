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

	//
	bool CreateSRV();

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

	// SRVがあるか？
	bool IsSRV() {
		if (m_srv.index < 0) {
			return false;
		}
		return true;
	}

	// RTVがあるか?
	bool IsRTV()
	{
		if (m_rtv.index < 0) {
			return false;
		}
		return true;
	}

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

	// フォーマット
	DXGI_FORMAT m_format = {};

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

	// レンダーターゲットの情報を設定
	// ※レンダーターゲット作成前に必ず呼ぶ
	void SetUp(const UINT rtNum, const UINT w, const UINT h);

	//================================================
	//
	// 作成関数
	//
	//================================================
	// 初期設定で指定したサイズ、枚数のRTテクスチャを作成する
	// ※フォーマットはすべて同じとなる
	bool CreateRT_All(DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM);

	// 1枚ずつレンダーターゲットを作成する
	// ※初期設定で決めた大きさ以外が設定できる(何も設定しない場合はデフォルトの大きさになる)
	// ※フォーマットもそれぞれ変更できる
	bool CreateRT(
		DXGI_FORMAT format,
		const UINT w = 0, const UINT h = 0);

	// ディスクリプタヒープの作成
	bool CreateDescriptorHeap();

	//================================================
	//
	// 設定関係
	//
	//================================================
	// レンダーターゲットを設定する
	void CreateCommand_SetRTV_All(ComPtr<ID3D12GraphicsCommandList>& comList);
	void CreateCommand_SetRTV(const UINT num, ComPtr<ID3D12GraphicsCommandList>& comList);

	// レンダーターゲットを解除する
	void CreateCommand_ResetRTV_All(ComPtr<ID3D12GraphicsCommandList>& comList);
	void CreateCommand_ResetRTV(const UINT num, ComPtr<ID3D12GraphicsCommandList>& comList);

	// シェーダリソースビューを設定する
	void CreateCommand_SetSRV(const UINT rtNum, const ComPtr<ID3D12GraphicsCommandList>& command, const UINT paraNum)const;
	//void CreateCommand_SetSRV_ALL(const UINT rtNum, const ComPtr<ID3D12GraphicsCommandList>& command, const UINT paraNum)const;

	// RTをクリアする
	void ClearRT_All(RgCommandList& comList, const RgVec4 clearColor);
	void ClearRt(const UINT num, RgCommandList& comList, const RgVec4 clearColor);

	//================================================
	//
	// 取得関係
	//
	//================================================

	// 
	void Release(){}

private:
	// 初期設定したか？
	bool m_IsSetUp = false;

	// 
	std::vector<std::shared_ptr<RgTexture2D>> m_rt;
	// RTV用ディスクリプタヒープハンドル
	CD3DX12_CPU_DESCRIPTOR_HANDLE m_rtvHandle;

	// マルチレンダーターゲットの枚数
	UINT m_multiRTCount = 1;
	// レンダーターゲットの大きさ
	UINT m_rtW;
	UINT m_rtH;

	// RTV用ディスクリプタヒープ
	ComPtr<ID3D12DescriptorHeap> m_heapRtv;
	//
	UINT m_rtvDescriptorSize = 0;
	// 使用している数
	UINT m_useDescriptorNum = 0;
};