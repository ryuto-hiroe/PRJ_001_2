

#include "RgLibrary.h"
#include "RgD3D.h"

bool RgD3D::Init(HWND hWnd, int w, int h)
{
	HRESULT hr;
	UINT dxgiFlags = 0;

#if defined(_DEBUG)

	ComPtr<ID3D12Debug> debug;
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debug))))
	{
		debug->EnableDebugLayer();
		dxgiFlags |= DXGI_CREATE_FACTORY_DEBUG;

#if 0
		ComPtr<ID3D12Debug3> debug3;
		debug.As(&debug3);
		if (debug3)
		{
			debug3->SetEnableGPUBasedValidation(true);
		}
#endif
	}
#endif

	ComPtr<IDXGIFactory3> factory;
	hr = CreateDXGIFactory2(dxgiFlags, IID_PPV_ARGS(&factory));
	if (FAILED(hr)){
		return false;
	}

	// �n�[�h�E�F�A�A�_�v�^�̌���
	ComPtr<IDXGIAdapter1> useAdapter;
	{
		UINT adapterIndex = 0;
		ComPtr<IDXGIAdapter1> adapter;
		while (DXGI_ERROR_NOT_FOUND != factory->EnumAdapters1(adapterIndex, &adapter))
		{
			DXGI_ADAPTER_DESC1 desc1{};
			adapter->GetDesc1(&desc1);
			++adapterIndex;
			if (desc1.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
				continue;

			// D3D12�͎g�p�\��
			hr = D3D12CreateDevice(
				adapter.Get(),
				D3D_FEATURE_LEVEL_12_0,
				__uuidof(ID3D12Device), nullptr);
			if (SUCCEEDED(hr))
				break;
		}
		adapter.As(&useAdapter); // �g�p����A�_�v�^�[
	}

	// �f�o�C�X����
	hr = D3D12CreateDevice(useAdapter.Get(), D3D_FEATURE_LEVEL_12_0, IID_PPV_ARGS(&m_device));
	if (FAILED(hr)) {
		return false;
	}

	// �R�}���h�L���[����
	D3D12_COMMAND_QUEUE_DESC queueDesc{
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		0,
		D3D12_COMMAND_QUEUE_FLAG_NONE,
		0
	};
	hr = m_device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&m_commandQueue));
	if (FAILED(hr)) {
		return false;
	}

	// HWND ����N���C�A���g�̈�T�C�Y�𔻒肷��B
	// (�E�B���h�E�T�C�Y��������Ă�����g�p����̂��悢)
	RECT rect;
	GetClientRect(hWnd, &rect);
	int width = rect.right - rect.left;
	int height = rect.bottom - rect.top;

	// �X���b�v�`�F�C���̐���
	{
		DXGI_SWAP_CHAIN_DESC1 scDesc{};
		scDesc.BufferCount		= FrameBufferCount;
		scDesc.Width			= width;
		scDesc.Height			= height;
		scDesc.Format			= DXGI_FORMAT_R8G8B8A8_UNORM;
		scDesc.BufferUsage		= DXGI_USAGE_RENDER_TARGET_OUTPUT;
		scDesc.SwapEffect		= DXGI_SWAP_EFFECT_FLIP_DISCARD;
		scDesc.SampleDesc.Count = 1;

		ComPtr<IDXGISwapChain1> swapchain;
		hr = factory->CreateSwapChainForHwnd(
			m_commandQueue.Get(),
			hWnd,
			&scDesc,
			nullptr,
			nullptr,
			&swapchain);
		if (FAILED(hr)) {
			return false;
		}
		swapchain.As(&m_swapchain); // IDXGISwapChain4 �擾
	}

	// �e�f�B�X�N���v�^�q�[�v�̏���.
	if (PrepareDescriptorHeaps() == false) {
		return false;
	}
	// �����_�[�^�[�Q�b�g�r���[�̐���.
	if (PrepareRenderTargetView() == false) {
		return false;
	}
	// �f�v�X�o�b�t�@�֘A�̏���.
	if (CreateDepthBuffer(width, height) == false) {
		return false;
	}
	// �R�}���h�A���P�[�^�|�̏���.
	if (CreateCommandAllocators() == false) {
		return false;
	}
	// �`��t���[�������p�t�F���X����.
	if (CreateFrameFences() == false) {
		return false;
	}
	// �R�}���h���X�g�̐���.
	hr = m_device->CreateCommandList(
		0,
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		m_commandAllocators2[0].Get(),
		nullptr,
		IID_PPV_ARGS(&m_texCommandList)
	);
	if (FAILED(hr)) {
		return false;
	}
	m_texCommandList->Close();

	//
	m_viewport = CD3DX12_VIEWPORT(0.0f, 0.0f, float(width), float(height));
	m_scissorRect = CD3DX12_RECT(0, 0, LONG(width), LONG(height));

	return true;
}

void RgD3D::Release()
{
}

void RgD3D::BeginFrame()
{
	m_frameIndex = m_swapchain->GetCurrentBackBufferIndex();

	//m_barrierToRT = CD3DX12_RESOURCE_BARRIER::Transition(
	//	m_renderTargets[m_frameIndex].Get(),
	//	D3D12_RESOURCE_STATE_PRESENT,
	//	D3D12_RESOURCE_STATE_RENDER_TARGET
	//);
	//
	//CD3DX12_CPU_DESCRIPTOR_HANDLE rtv(
	//	m_heapRtv->GetCPUDescriptorHandleForHeapStart(),
	//	m_frameIndex, m_rtvDescriptorSize);
	//m_rtv = rtv;
	//
	//CD3DX12_CPU_DESCRIPTOR_HANDLE dsv(
	//	m_heapDsv->GetCPUDescriptorHandleForHeapStart()
	//);
	//m_dsv = dsv;
}

void RgD3D::SetBackBuffer(RgCommandList& comList)
{
	// �X���b�v�`�F�C���\���\���烌���_�[�^�[�Q�b�g�`��\��
	auto barrierToRT = CD3DX12_RESOURCE_BARRIER::Transition(
		m_renderTargets[m_frameIndex].Get(),
		D3D12_RESOURCE_STATE_PRESENT,
		D3D12_RESOURCE_STATE_RENDER_TARGET
	);
	comList.GetCommandList()->ResourceBarrier(1, &barrierToRT);

	CD3DX12_CPU_DESCRIPTOR_HANDLE rtv(
		m_heapRtv->GetCPUDescriptorHandleForHeapStart(),
		m_frameIndex, m_rtvDescriptorSize);
	CD3DX12_CPU_DESCRIPTOR_HANDLE dsv(
		m_heapDsv->GetCPUDescriptorHandleForHeapStart()
	);

	// �`�����Z�b�g
	comList.GetCommandList()->OMSetRenderTargets(1, &rtv, FALSE, &dsv);
	// �r���[�|�[�g�ƃV�U�[�̃Z�b�g
	comList.GetCommandList()->RSSetViewports(1, &m_viewport);
	comList.GetCommandList()->RSSetScissorRects(1, &m_scissorRect);
}

void RgD3D::ResetBackBuffer(RgCommandList& comList)
{
	// �����_�[�^�[�Q�b�g����X���b�v�`�F�C���\���\��
	auto barrierToPresent = CD3DX12_RESOURCE_BARRIER::Transition(
		m_renderTargets[m_frameIndex].Get(),
		D3D12_RESOURCE_STATE_RENDER_TARGET,
		D3D12_RESOURCE_STATE_PRESENT
	);
	comList.GetCommandList()->ResourceBarrier(1, &barrierToPresent);
}

void RgD3D::ClearBackBuffer(RgCommandList& comList, const RgVec4 clearColor)
{
	CD3DX12_CPU_DESCRIPTOR_HANDLE rtv(
		m_heapRtv->GetCPUDescriptorHandleForHeapStart(),
		m_frameIndex, m_rtvDescriptorSize);
	CD3DX12_CPU_DESCRIPTOR_HANDLE dsv(
		m_heapDsv->GetCPUDescriptorHandleForHeapStart()
	);

	// �J���[�o�b�t�@(�����_�[�^�[�Q�b�g�r���[)�̃N���A
	comList.GetCommandList()->ClearRenderTargetView(rtv, &clearColor.x, 0, nullptr);
	// �f�v�X�o�b�t�@(�f�v�X�X�e���V���r���[)�̃N���A
	comList.GetCommandList()->ClearDepthStencilView(
		dsv, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
}


void RgD3D::WaitPreviousFrame()
{
	// ���݂̃t�F���X�� GPU �����B��ݒ肳���l���Z�b�g.
	auto& fence = m_frameFences[m_frameIndex];
	const auto currentValue = ++m_frameFenceValues[m_frameIndex];
	m_commandQueue->Signal(fence.Get(), currentValue);

	// ����������R�}���h�i�A���P�[�^�|�j�̂��͎̂��s�����ς݂����A
	// �΂ɂȂ��Ă���t�F���X�Ŋm�F����.
	auto nextIndex = (m_frameIndex + 1) % FrameBufferCount;
	const auto finishExpected = m_frameFenceValues[nextIndex];
	const auto nextFenceValue = m_frameFences[nextIndex]->GetCompletedValue();
	if (nextFenceValue < finishExpected)
	{
		// GPU ���������̂��߁A�C�x���g�őҋ@����
		m_frameFences[nextIndex]->SetEventOnCompletion(finishExpected, m_fenceWaitEvent);
		WaitForSingleObject(m_fenceWaitEvent, GpuWaitTimeout);
	}
}

void RgD3D::WaitGPU()
{
	HRESULT hr;
	const auto finishExpected = m_frameFenceValues[m_frameIndex];
	hr = m_commandQueue->Signal(m_frameFences[m_frameIndex].Get(), finishExpected);
	if (FAILED(hr))
	{
		throw std::runtime_error("Failed Signal(WaitGPU)");
	}
	m_frameFences[m_frameIndex]->SetEventOnCompletion(finishExpected, m_fenceWaitEvent);
	WaitForSingleObject(m_fenceWaitEvent, GpuWaitTimeout);
	m_frameFenceValues[m_frameIndex] = finishExpected + 1;
}

bool RgD3D::PrepareDescriptorHeaps()
{
	// RTV �̃f�B�X�N���v�^�q�[�v
	HRESULT hr;
	D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc{
	  D3D12_DESCRIPTOR_HEAP_TYPE_RTV,
	  FrameBufferCount,
	  D3D12_DESCRIPTOR_HEAP_FLAG_NONE,
	  0
	};
	hr = m_device->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&m_heapRtv));
	if (FAILED(hr)) {
		return false;
	}
	m_rtvDescriptorSize = m_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

	// DSV �̃f�B�X�N���v�^�q�[�v
	D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc{
	  D3D12_DESCRIPTOR_HEAP_TYPE_DSV,
	  1,
	  D3D12_DESCRIPTOR_HEAP_FLAG_NONE,
	  0
	};
	hr = m_device->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(&m_heapDsv));
	if (FAILED(hr)){
		return false;
	}

	return true;
}
bool RgD3D::PrepareRenderTargetView()
{
	// �X���b�v�`�F�C���C���[�W�ւ̃����_�[�^�[�Q�b�g�r���[����
	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(
		m_heapRtv->GetCPUDescriptorHandleForHeapStart());
	for (UINT i = 0; i < FrameBufferCount; ++i)
	{
		m_swapchain->GetBuffer(i, IID_PPV_ARGS(&m_renderTargets[i]));
		m_device->CreateRenderTargetView(m_renderTargets[i].Get(), nullptr, rtvHandle);
		// �Q�Ƃ���f�B�X�N���v�^�̕ύX
		rtvHandle.Offset(1, m_rtvDescriptorSize);
	}

	return true;
}
bool RgD3D::CreateDepthBuffer(int width, int height)
{
	// �f�v�X�o�b�t�@�̐���
	auto depthBufferDesc = CD3DX12_RESOURCE_DESC::Tex2D(
		DXGI_FORMAT_D32_FLOAT,
		width,
		height,
		1, 0,
		1, 0,
		D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL
	);
	D3D12_CLEAR_VALUE depthClearValue{};
	depthClearValue.Format = depthBufferDesc.Format;
	depthClearValue.DepthStencil.Depth = 1.0f;
	depthClearValue.DepthStencil.Stencil = 0;

	HRESULT hr;
	hr = m_device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
		D3D12_HEAP_FLAG_NONE,
		&depthBufferDesc,
		D3D12_RESOURCE_STATE_DEPTH_WRITE,
		&depthClearValue,
		IID_PPV_ARGS(&m_depthBuffer)
	);
	if (FAILED(hr)) {
		return false;
	}

	// �f�v�X�X�e���V���r���[����
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc
	{
		DXGI_FORMAT_D32_FLOAT,  // Format
		D3D12_DSV_DIMENSION_TEXTURE2D,  // ViewDimension
		D3D12_DSV_FLAG_NONE,    // Flags
		{ // D3D12_TEX2D_DSV
			0 // MipSlice
		}
	};
	CD3DX12_CPU_DESCRIPTOR_HANDLE dsvHandle(m_heapDsv->GetCPUDescriptorHandleForHeapStart());
	m_device->CreateDepthStencilView(m_depthBuffer.Get(), &dsvDesc, dsvHandle);

	return true;
}
bool RgD3D::CreateCommandAllocators()
{
	HRESULT hr;
	//m_commandAllocators.resize(FrameBufferCount);
	//for (UINT i = 0; i < FrameBufferCount; ++i)
	//{
	//	hr = m_device->CreateCommandAllocator(
	//		D3D12_COMMAND_LIST_TYPE_DIRECT,
	//		IID_PPV_ARGS(&m_commandAllocators[i])
	//	);
	//	if (FAILED(hr)) {
	//		return false;
	//	}
	//}

	m_commandAllocators2.resize(FrameBufferCount);
	for (UINT i = 0; i < FrameBufferCount; ++i)
	{
		hr = m_device->CreateCommandAllocator(
			D3D12_COMMAND_LIST_TYPE_DIRECT,
			IID_PPV_ARGS(&m_commandAllocators2[i])
		);
		if (FAILED(hr)) {
			return false;
		}
	}
	return true;
}
bool RgD3D::CreateFrameFences()
{
	HRESULT hr;
	m_frameFences.resize(FrameBufferCount);
	for (UINT i = 0; i < FrameBufferCount; ++i)
	{
		hr = m_device->CreateFence(
			0,  // �����l
			D3D12_FENCE_FLAG_NONE,
			IID_PPV_ARGS(&m_frameFences[i]));
		if (FAILED(hr)) {
			return false;
		}
	}

	return true;
}


HRESULT RgD3D::CompileShaderFromFile(
	const std::wstring& fileName,
	const std::wstring& profile,
	ComPtr<ID3DBlob>& shaderBlob,
	ComPtr<ID3DBlob>& errorBlob)
{
	using namespace std::filesystem;

	path filePath(fileName);
	std::ifstream infile(filePath);
	std::vector<char> srcData;
	if (!infile)
		throw std::runtime_error("shader not found");
	srcData.resize(uint32_t(infile.seekg(0, infile.end).tellg()));
	infile.seekg(0, infile.beg).read(srcData.data(), srcData.size());

	// DXC �ɂ��R���p�C������
	ComPtr<IDxcLibrary> library;
	ComPtr<IDxcCompiler> compiler;
	ComPtr<IDxcBlobEncoding> source;
	ComPtr<IDxcOperationResult> dxcResult;

	DxcCreateInstance(CLSID_DxcLibrary, IID_PPV_ARGS(&library));
	library->CreateBlobWithEncodingFromPinned(srcData.data(), UINT(srcData.size()), CP_ACP, &source);
	DxcCreateInstance(CLSID_DxcCompiler, IID_PPV_ARGS(&compiler));

	LPCWSTR compilerFlags[] = {
  #if _DEBUG
	  L"/Zi", L"/O0",
  #else
	  L"/O2" // �����[�X�r���h�ł͍œK��
  #endif
	};
	compiler->Compile(source.Get(), filePath.wstring().c_str(),
		L"main", profile.c_str(),
		compilerFlags, _countof(compilerFlags),
		nullptr, 0, // Defines
		nullptr,
		&dxcResult);

	HRESULT hr;
	dxcResult->GetStatus(&hr);
	if (SUCCEEDED(hr))
	{
		dxcResult->GetResult(
			reinterpret_cast<IDxcBlob**>(shaderBlob.GetAddressOf())
		);
	}
	else
	{
		dxcResult->GetErrorBuffer(
			reinterpret_cast<IDxcBlobEncoding**>(errorBlob.GetAddressOf())
		);
	}
	return hr;
}