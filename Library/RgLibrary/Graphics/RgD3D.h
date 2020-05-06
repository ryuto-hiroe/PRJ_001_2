#pragma once

//===============================================================
//
// Direct3D管理クラス
//
//===============================================================
class RgD3D
{
public:

	// Direct3D初期化
	bool Init(HWND hWnd, int w, int h);
	// 解放
	void Release();
	// GPUと同期する
	void WaitPreviousFrame();
	// GPUを待機する
	void WaitGPU();

	// フレーム開始処理
	void BeginFrame();

	// バックバッファの設定
	void SetBackBuffer(RgCommandList& comList);

	// バックバッファを表示可能にする
	void ResetBackBuffer(RgCommandList& comList);

	// バックバッファをクリア
	void ClearBackBuffer(RgCommandList& comList, const RgVec4 clearColor = { 0.1f,0.25f,0.5f,1.0f });

	//============================================
	// シェーダコンパイラ(shader model 6)
	//============================================
	HRESULT CompileShaderFromFile(
		const std::wstring& fileName, 
		const std::wstring& profile, 
		ComPtr<ID3DBlob>& shaderBlob, 
		ComPtr<ID3DBlob>& errorBlob);

	//============================================
	// 取得関数
	//============================================
	// デバイスの取得
	ComPtr<ID3D12Device>& GetDevice() { return m_device; }
	// コマンドキューの取得
	ComPtr<ID3D12CommandQueue>& GetCommandQueue() { return m_commandQueue; }
	// スワップチェインの取得
	ComPtr<IDXGISwapChain4>& GetSwapChain() { return m_swapchain; }


	const UINT GpuWaitTimeout = (10 * 1000);  // 10s
	const UINT FrameBufferCount = 3;

	ComPtr<ID3D12Device> m_device;
	ComPtr<ID3D12CommandQueue> m_commandQueue;
	ComPtr<IDXGISwapChain4> m_swapchain;

	ComPtr<ID3D12DescriptorHeap> m_heapRtv;
	ComPtr<ID3D12DescriptorHeap> m_heapDsv;

	std::vector<ComPtr<ID3D12Resource1>> m_renderTargets;
	ComPtr<ID3D12Resource1> m_depthBuffer;

	CD3DX12_VIEWPORT  m_viewport;
	CD3DX12_RECT m_scissorRect;

	UINT m_rtvDescriptorSize;
	std::vector<ComPtr<ID3D12CommandAllocator>> m_commandAllocators2;

	HANDLE m_fenceWaitEvent;
	std::vector<ComPtr<ID3D12Fence1>> m_frameFences;
	std::vector<UINT64> m_frameFenceValues;

	ComPtr<ID3D12GraphicsCommandList> m_texCommandList;

	CD3DX12_CPU_DESCRIPTOR_HANDLE m_rtv;
	CD3DX12_CPU_DESCRIPTOR_HANDLE m_dsv;
	CD3DX12_RESOURCE_BARRIER m_barrierToRT;


	UINT m_frameIndex;

	//============================================
	// 作成・準備関数
	//============================================
	bool PrepareDescriptorHeaps();
	bool PrepareRenderTargetView();
	bool CreateDepthBuffer(int width, int height);
	bool CreateCommandAllocators();
	bool CreateFrameFences();

	//============================================
	// シングルトンパターン
	//============================================
private:
	RgD3D()
	{
		m_renderTargets.resize(FrameBufferCount);
		m_frameFenceValues.resize(FrameBufferCount);
		m_frameIndex = 0;

		m_fenceWaitEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	}

public:
	static RgD3D& GetInstance() {
		static RgD3D Instance;
		return Instance;
	}
};
#define RGD3D RgD3D::GetInstance()