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
// SRV�ACBV�AUAV��DescriptorHeap���Ǘ�����N���X
//
//	 �ECreate�֐��Ŋe�r���[�̍쐬���ł���
//	 �Em_freeIndices�ɓ����Ă�����̂��g�p�ł���Heap�ƂȂ��Ă���
//	 �Em_heapCount�𒴂���ꏊ�ɃA�N�Z�X����ƃG���[�������邵�悤�ƂȂ��Ă���
//
//================================================
class RgSrvCbvUavHeapManager
{
public:

	//==================================================
	//
	// �쐬�֌W
	//
	//==================================================
	// SRV�̍쐬
	// resource�@�c�@���\�[�X
	// desc�@�@�@�c�@SRV�쐬�p�\����
	RgHeapHandle CreateSRV(
		ComPtr<ID3D12Resource1>& resource, 
		const D3D12_SHADER_RESOURCE_VIEW_DESC* desc
	);

	// CBV�̍쐬
	// desc		�c�@CBV�쐬�p�\����
	RgHeapHandle CreateCBV(const D3D12_CONSTANT_BUFFER_VIEW_DESC* desc);

	// DescriptorHeap�̍쐬
	bool CreateDescriptorHeap();

	//==================================================
	//
	// �擾�֌W
	//
	//==================================================
	// DescriptorHeap�{�̂��擾
	ComPtr<ID3D12DescriptorHeap> GetDescriptorHeap() { return m_heap; }

	// �����쐬�����Ɏg�p�\��CPU�AGPU�̃n���h�����擾
	//	 ���擾����Handle��Index�͎g�p���ƂȂ�
	RgCpuAndGpuHandle GetHeapHandle();

	//==================================================
	//
	// ���̑�
	//
	//==================================================
	// �g�p���Ȃ��Ȃ���index���g�p�\�ɂ���
	void Free(const UINT index);

	// �R�}���h���쐬
	void MakeCommand(ComPtr<ID3D12GraphicsCommandList> comList);

	// ���
	void Release();

private:
	// �f�B�X�N���v�^�q�[�v�쐬��(NumDescriptors)
	const UINT m_heapCount = 2000;
	// �f�B�X�N���v�^�q�[�v�̖{��
	ComPtr<ID3D12DescriptorHeap> m_heap = nullptr;
	UINT m_heapSize = 0;
	// �g�p�ł���Index�̔z��
	std::list<UINT> m_freeIndices;


	//==================================================
	//
	// �V���O���g���p�^�[��
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
// RTV�p�f�B�X�N���v�^�q�[�v�Ǘ��N���X
//
//================================================
class RgRtvHeapManager
{
public:

	//==================================================
	//
	// �쐬�֌W
	//
	//==================================================

	// RTV�̍쐬
	RgHeapHandle CreateRTV(
		ComPtr<ID3D12Resource1>& resource,
		const D3D12_SHADER_RESOURCE_VIEW_DESC* desc);

	// DescriptorHeap�̍쐬
	bool CreateDescriptorHeap();

	//==================================================
	//
	// �擾�֌W
	//
	//==================================================
	// DescriptorHeap�{�̂��擾
	ComPtr<ID3D12DescriptorHeap> GetDescriptorHeap() { return m_heap; }

	// �����쐬�����Ɏg�p�\��CPU�AGPU�̃n���h�����擾
	//	 ���擾����Handle��Index�͎g�p���ƂȂ�
	RgCpuAndGpuHandle GetHeapHandle();

	//==================================================
	//
	// ���̑�
	//
	//==================================================
	// �g�p���Ȃ��Ȃ���index���g�p�\�ɂ���
	void Free(const UINT index);

	// �R�}���h���쐬
	void MakeCommand(ComPtr<ID3D12GraphicsCommandList> comList);

	// ���
	void Release();

private:
	// �f�B�X�N���v�^�q�[�v�쐬��(NumDescriptors)
	const UINT m_heapCount = 100;
	// �f�B�X�N���v�^�q�[�v�̖{��
	ComPtr<ID3D12DescriptorHeap> m_heap = nullptr;
	UINT m_heapSize = 0;
	// �g�p�ł���Index�̔z��
	std::list<UINT> m_freeIndices;

	//==================================================
	//
	// �V���O���g���p�^�[��
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