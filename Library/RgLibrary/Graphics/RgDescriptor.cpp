
#include "RgLibrary.h"
#include "RgDescriptor.h"


RgHeapHandle RgSrvCbvUavHeapManager::CreateSRV(
	ComPtr<ID3D12Resource1>& resource,
	const D3D12_SHADER_RESOURCE_VIEW_DESC* desc
)
{
	RgHeapHandle result;

	if (m_freeIndices.size() > 0)
	{
		UINT index = m_freeIndices.front();
		m_freeIndices.pop_front();

		auto cpuHandle = CD3DX12_CPU_DESCRIPTOR_HANDLE(
			m_heap->GetCPUDescriptorHandleForHeapStart(),
			index, m_heapSize);

		// SRV�̍쐬
		RGD3D.GetDevice()->CreateShaderResourceView(
			resource.Get(), desc, cpuHandle);
		//
		result.handle = CD3DX12_GPU_DESCRIPTOR_HANDLE(
			m_heap->GetGPUDescriptorHandleForHeapStart(), 
			index, m_heapSize);
		result.index = index;
	}
	else {
		// �쐬���s
		throw std::runtime_error("Heap size is not enough");
	}

	return result;
}

RgHeapHandle RgSrvCbvUavHeapManager::CreateCBV(const D3D12_CONSTANT_BUFFER_VIEW_DESC* desc)
{
	RgHeapHandle result;

	if (m_freeIndices.size() > 0)
	{
		UINT index = m_freeIndices.front();
		m_freeIndices.pop_front();

		auto cpuHandle = CD3DX12_CPU_DESCRIPTOR_HANDLE(
			m_heap->GetCPUDescriptorHandleForHeapStart(),
			index, m_heapSize);

		// CBV�̍쐬
		RGD3D.GetDevice()->CreateConstantBufferView(desc, cpuHandle);

		//
		result.handle = CD3DX12_GPU_DESCRIPTOR_HANDLE(
			m_heap->GetGPUDescriptorHandleForHeapStart(),
			index, m_heapSize);
		result.index = index;
	}
	else {
		// �쐬���s
		throw std::runtime_error("Heap size is not enough");
	}

	return result;
}

bool RgSrvCbvUavHeapManager::CreateDescriptorHeap()
{
	if (m_heap)return false;

	// SRV�ACBV�p�f�B�X�N���v�^�q�[�v�̍쐬
	D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc{
		D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV,
		m_heapCount,
		D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE,
		0
	};

	auto hr = RGD3D.m_device->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&m_heap));
	if (FAILED(hr)) {
		return false;
	}
	m_heapSize = RGD3D.m_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	// �g�p�ł���C���f�b�N�X��o�^
	for (UINT i = 0; i < m_heapCount; i++) {
		m_freeIndices.push_back(i);
	}

	return true;
}

RgCpuAndGpuHandle RgSrvCbvUavHeapManager::GetHeapHandle()
{
	RgCpuAndGpuHandle res;

	if (m_freeIndices.size() > 0)
	{
		UINT index = m_freeIndices.front();
		m_freeIndices.pop_front();

		res.cpuhandle = CD3DX12_CPU_DESCRIPTOR_HANDLE(
			m_heap->GetCPUDescriptorHandleForHeapStart(),
			index, m_heapSize);
		res.gpuhandle = CD3DX12_GPU_DESCRIPTOR_HANDLE(
			m_heap->GetGPUDescriptorHandleForHeapStart(),
			index, m_heapSize);
	}
	else {
		// �쐬���s
		throw std::runtime_error("Heap size is not enough");
	}

	return res;
}

void RgSrvCbvUavHeapManager::Free(const UINT index)
{
	// ���łɎg�p�\�ɂȂ��Ă��邩���m�F����
	bool IsOk = true;
	for (auto&& num : m_freeIndices) {
		if (num == index) {
			IsOk = false;
			break;
		}
	}

	if (IsOk) {
		// �g�p�\���X�g�ɒǉ�����
		m_freeIndices.push_back(index);
	}
}

void RgSrvCbvUavHeapManager::MakeCommand(ComPtr<ID3D12GraphicsCommandList> comList)
{
	// �f�B�X�N���v�^�q�[�v���Z�b�g
	ID3D12DescriptorHeap* heaps[] = {
		m_heap.Get()
	};
	comList->SetDescriptorHeaps(_countof(heaps), heaps);
}

void RgSrvCbvUavHeapManager::Release()
{
	m_freeIndices.clear();
}


bool RgRtvHeapManager::CreateDescriptorHeap()
{
	HRESULT hr;
	D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc{
		D3D12_DESCRIPTOR_HEAP_TYPE_RTV,
		m_heapCount,		// �����_�[�^�[�Q�b�g��
		D3D12_DESCRIPTOR_HEAP_FLAG_NONE,
		0
	};

	hr = RGD3D.GetDevice()->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&m_heap));
	if (FAILED(hr)) {
		return false;
	}
	m_heapSize = RGD3D.GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

	for (UINT i = 0; i < m_heapCount; i++) {
		m_freeIndices.push_back(i);
	}

	return true;
}

RgHeapHandle RgRtvHeapManager::CreateRTV(
	ComPtr<ID3D12Resource1>& resource,
	const D3D12_SHADER_RESOURCE_VIEW_DESC* desc)
{
	RgHeapHandle result;


	return result;
}