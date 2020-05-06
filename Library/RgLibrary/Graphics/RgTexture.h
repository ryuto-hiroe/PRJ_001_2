#pragma once


//===============================================================
//
// �e�N�X�`���N���X
//
//		�ERgTextureLoader
//		�ERgTexture2D
//		�ERgTexture3D
//
//===============================================================


//===============================================================
//
// 2D�e�N�X�`���̃N���X
//
//===============================================================
class RgTexture2D
{
public:

	//================================================
	//
	// �ݒ�֌W
	//
	//================================================
	// �R�}���h�쐬
	void CreateCommandSetSRV(const ComPtr<ID3D12GraphicsCommandList>& command, const UINT paraNum)const;

	//================================================
	//
	// �ǂݍ��݁A�쐬�֐�
	//
	//================================================

	// �t�@�C������e�N�X�`���ǂݍ���
	bool LoadTexture(const std::string& filename);

	// ����������e�N�X�`����ǂݍ���
	bool LoadTextureFromMemory(const std::vector<char>& imageData);

	
	// RTV�t�e�N�X�`���𐶐�
	bool CreateTexAndSRV(const int w, const int h, DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM);


	// �e�N�X�`�����g�p�ł���`�ɕύX����
	bool SetUpTexture(
		const DirectX::ScratchImage& image,
		const DirectX::TexMetadata& mate);

	//================================================
	//
	// �擾�֌W
	//
	//================================================
	// ���\�[�X���擾
	const ComPtr<ID3D12Resource1> GetResource() { return m_texture2D; }

	//
	void Release();

private:

	//================================================
	//
	// �f�[�^
	//
	//================================================
	// �t�@�C����(LoadTexture�����ꍇ�̂�)
	std::string			m_filepath;

	// �e�N�X�`�����\�[�X
	ComPtr<ID3D12Resource1>			m_texture2D = nullptr;

	//-------------------------
	// SRV
	//-------------------------
	// SRV�쐬�p�\����
	D3D12_SHADER_RESOURCE_VIEW_DESC m_srvDesc = {};
	// �n���h��
	//	12�ł́A�f�B�X�N���v�^�q�[�v��View���쐬���A�g���Ƃ���
	//	�f�B�X�N���v�^�q�[�v�ɍ쐬�����ꏊ�̃n���h�����g�p����
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
// Render Target�̃N���X
//	�ERT�e�N�X�`�����쐬���ĊǗ�����N���X
//	�E�}���`�����_�[�^�[�Q�b�g�Ή�
//
//===============================================================
class RgRenderTargets
{
public:

	//================================================
	//
	// �쐬�֐�
	//
	//================================================
	// �����_�[�^�[�Q�b�g�쐬
	//	�E�}���`�����_�[�^�[�Q�b�g�Ή�
	bool CreateRT(
		const int w,
		const int h,
		std::vector<DXGI_FORMAT> format = { DXGI_FORMAT_R8G8B8A8_UNORM }
	);

	// �f�B�X�N���v�^�q�[�v�̍쐬
	bool CreateDescriptorHeap();

	//================================================
	//
	// �ݒ�֌W
	//
	//================================================
	// �����_�[�^�[�Q�b�g��ݒ肷��
	void SetRenderTargets(ComPtr<ID3D12GraphicsCommandList>& comList);

	// �V�F�[�_���\�[�X�r���[��ݒ肷��
	void CreateCommandSetSRV(const UINT rtNum, const ComPtr<ID3D12GraphicsCommandList>& command, const UINT paraNum)const;

	// RT���N���A����
	//void ClearRT()

	//================================================
	//
	// �擾�֌W
	//
	//================================================

	// 
	void Release(){}

private:
	// 
	std::vector<std::shared_ptr<RgTexture2D>> m_rt;
	// RTV�p�f�B�X�N���v�^�q�[�v�n���h��
	std::vector<CD3DX12_CPU_DESCRIPTOR_HANDLE> m_rtvHandles;

	// �}���`�����_�[�^�[�Q�b�g�̖���
	UINT m_multiRTCount = 1;

	// RTV�p�f�B�X�N���v�^�q�[�v
	ComPtr<ID3D12DescriptorHeap> m_heapRtv;
	//
	UINT m_rtvDescriptorSize = 0;
	// �g�p���Ă��鐔
	UINT m_useDescriptorNum = 0;
};