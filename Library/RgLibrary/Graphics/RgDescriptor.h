#pragma once


struct RgHeapHandle
{
	D3D12_GPU_DESCRIPTOR_HANDLE handle = {};
	D3D12_GPU_DESCRIPTOR_HANDLE gpuhandle = {};
	int index = -1;
};

struct RgCpuAndGpuHandle
{
	D3D12_CPU_DESCRIPTOR_HANDLE cpuhandle = {};
	D3D12_GPU_DESCRIPTOR_HANDLE gpuhandle = {};
	int index = -1;
};

//================================================
//
// SRV、CBV、UAVのDescriptorHeapを管理するクラス
//
//	 ・Create関数で各ビューの作成ができる
//	 ・m_freeIndicesに入っているものが使用できるHeapとなっている
//	 ・m_heapCountを超える場所にアクセスするとエラー落ちするしようとなっている
//
//================================================
class RgSrvCbvUavHeapManager
{
public:

	//==================================================
	//
	// 作成関係
	//
	//==================================================
	// SRVの作成
	// resource　…　リソース
	// desc　　　…　SRV作成用構造体
	RgHeapHandle CreateSRV(
		ComPtr<ID3D12Resource1>& resource, 
		const D3D12_SHADER_RESOURCE_VIEW_DESC* desc
	);

	// CBVの作成
	// desc		…　CBV作成用構造体
	RgHeapHandle CreateCBV(const D3D12_CONSTANT_BUFFER_VIEW_DESC* desc);

	// DescriptorHeapの作成
	bool CreateDescriptorHeap();

	//==================================================
	//
	// 取得関係
	//
	//==================================================
	// DescriptorHeap本体を取得
	ComPtr<ID3D12DescriptorHeap> GetDescriptorHeap() { return m_heap; }

	// 何も作成せずに使用可能なCPU、GPUのハンドルを取得
	//	 ※取得したHandleのIndexは使用中となる
	RgCpuAndGpuHandle GetHeapHandle();

	//==================================================
	//
	// その他
	//
	//==================================================
	// 使用しなくなったindexを使用可能にする
	void Free(const UINT index);

	// コマンドを作成
	void MakeCommand(ComPtr<ID3D12GraphicsCommandList> comList);

	// 解放
	void Release();

private:
	// ディスクリプタヒープ作成数(NumDescriptors)
	const UINT m_heapCount = 2000;
	// ディスクリプタヒープの本体
	ComPtr<ID3D12DescriptorHeap> m_heap = nullptr;
	UINT m_heapSize = 0;
	// 使用できるIndexの配列
	std::list<UINT> m_freeIndices;


	//==================================================
	//
	// シングルトンパターン
	//
	//==================================================
private:
	RgSrvCbvUavHeapManager(){}

public:
	static RgSrvCbvUavHeapManager& GetInstance() {
		static RgSrvCbvUavHeapManager ins;
		return ins;
	}
};
#define RGHEAPMGR RgSrvCbvUavHeapManager::GetInstance()





//================================================
//
// RTV用ディスクリプタヒープ管理クラス
//
//================================================
class RgRtvHeapManager
{
public:

	//==================================================
	//
	// 作成関係
	//
	//==================================================

	// RTVの作成
	RgHeapHandle CreateRTV(
		ComPtr<ID3D12Resource1>& resource,
		const D3D12_SHADER_RESOURCE_VIEW_DESC* desc);

	// DescriptorHeapの作成
	bool CreateDescriptorHeap();

	//==================================================
	//
	// 取得関係
	//
	//==================================================
	// DescriptorHeap本体を取得
	ComPtr<ID3D12DescriptorHeap> GetDescriptorHeap() { return m_heap; }

	// 何も作成せずに使用可能なCPU、GPUのハンドルを取得
	//	 ※取得したHandleのIndexは使用中となる
	RgCpuAndGpuHandle GetHeapHandle();

	//==================================================
	//
	// その他
	//
	//==================================================
	// 使用しなくなったindexを使用可能にする
	void Free(const UINT index);

	// コマンドを作成
	void MakeCommand(ComPtr<ID3D12GraphicsCommandList> comList);

	// 解放
	void Release();

private:
	// ディスクリプタヒープ作成数(NumDescriptors)
	const UINT m_heapCount = 100;
	// ディスクリプタヒープの本体
	ComPtr<ID3D12DescriptorHeap> m_heap = nullptr;
	UINT m_heapSize = 0;
	// 使用できるIndexの配列
	std::list<UINT> m_freeIndices;

	//==================================================
	//
	// シングルトンパターン
	//
	//==================================================
private:
	//
	RgRtvHeapManager(){}

public:

	static RgRtvHeapManager& GetInstance() {
		static RgRtvHeapManager ins;
		return ins;
	}
};
#define RTVMGR RgRtvHeapManager::GetInstance()