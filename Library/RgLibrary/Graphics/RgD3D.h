#pragma once

//===============================================================
//
// Direct3D�Ǘ��N���X
//
//===============================================================
class RgD3D
{
public:

	// Direct3D������
	bool Init(HWND hWnd, int w, int h);
	// ���
	void Release();
	// GPU�Ɠ�������
	void WaitPreviousFrame();
	// GPU��ҋ@����
	void WaitGPU();

	// �t���[���J�n����
	void BeginFrame();

	// �o�b�N�o�b�t�@�̐ݒ�
	void SetBackBuffer(RgCommandList& comList);

	// �o�b�N�o�b�t�@��\���\�ɂ���
	void ResetBackBuffer(RgCommandList& comList);

	// �o�b�N�o�b�t�@���N���A
	void ClearBackBuffer(RgCommandList& comList, const RgVec4 clearColor = { 0.1f,0.25f,0.5f,1.0f });

	//============================================
	// �V�F�[�_�R���p�C��(shader model 6)
	//============================================
	HRESULT CompileShaderFromFile(
		const std::wstring& fileName, 
		const std::wstring& profile, 
		ComPtr<ID3DBlob>& shaderBlob, 
		ComPtr<ID3DBlob>& errorBlob);

	//============================================
	// �擾�֐�
	//============================================
	// �f�o�C�X�̎擾
	ComPtr<ID3D12Device>& GetDevice() { return m_device; }
	// �R�}���h�L���[�̎擾
	ComPtr<ID3D12CommandQueue>& GetCommandQueue() { return m_commandQueue; }
	// �X���b�v�`�F�C���̎擾
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
	// �쐬�E�����֐�
	//============================================
	bool PrepareDescriptorHeaps();
	bool PrepareRenderTargetView();
	bool CreateDepthBuffer(int width, int height);
	bool CreateCommandAllocators();
	bool CreateFrameFences();

	//============================================
	// �V���O���g���p�^�[��
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