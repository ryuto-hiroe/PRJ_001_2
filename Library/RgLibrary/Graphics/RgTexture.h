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
	void CreateCommandSetSRV(const UINT paraNum)const;

	// リソースを外部から設定する
	void SetResource(ComPtr<ID3D12Resource1> resource, bool isSrv = false);

	// リソースバリアをRT可能状態にする
	void BarrierToRT();
	// リソースバリアをスワップチェイン表示可能状態にする
	void BarrierToPresent();

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

	// テクスチャ情報構造体取得
	const D3D12_RESOURCE_DESC& GetInfo() { return m_desc; }

	// SRVがあるか？
	bool IsSRV() {
		if (m_srv.index < 0) {
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
	std::string				m_filepath;		// ファイル名(LoadTextureした場合のみ)
	ComPtr<ID3D12Resource1>	m_texture2D = nullptr;// テクスチャリソース
	D3D12_RESOURCE_DESC		m_desc = {};	// リソース情報構造体

	//-------------------------
	// SRV
	//-------------------------
	// SRV作成用構造体
	D3D12_SHADER_RESOURCE_VIEW_DESC m_srvDesc = {};
	// ハンドル
	//	12では、ディスクリプタヒープにViewを作成し、使うときは
	//	ディスクリプタヒープに作成した場所のハンドルを使用する
	RgHeapHandle m_srv;
};


// 
class RgRenderTargetViews
{
public:

	// RTVを作成する
	bool CreateRTVs(const std::vector<ComPtr<ID3D12Resource1>>& resources);

	// RTV Descriptor Handle 取得
	const D3D12_CPU_DESCRIPTOR_HANDLE GetRTV_Desc_Handle()const {
		return m_heapRtv->GetCPUDescriptorHandleForHeapStart();
	}

	// RTV枚数取得
	const UINT GetRTCount()const { return m_multiRTCount; }

private:
	// ディスクリプタヒープの作成
	bool CreateDescriptorHeap();

private:
	UINT m_multiRTCount = 1;				// RTVの数
	ComPtr<ID3D12DescriptorHeap> m_heapRtv;	// RTV用ディスクリプタヒープ
	UINT m_rtvDescriptorOffset = 0;			// RTVの一つの大きさ
};


class RgRTChanger
{
public:

	// 設定関係
	// RTVを設定
	void SetRTV(RgRenderTargetViews* rtv);
	// DSVを設定
	void SetDSV(const D3D12_CPU_DESCRIPTOR_HANDLE& dsv);

	// 
	void CreateCommand_SetRtvDsv(const ComPtr<ID3D12GraphicsCommandList>& command);

private:
	D3D12_CPU_DESCRIPTOR_HANDLE m_depthHandle = {};
	RgRenderTargetViews* m_rtvs = nullptr;
};