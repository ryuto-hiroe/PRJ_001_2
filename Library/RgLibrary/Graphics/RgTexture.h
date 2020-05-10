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
	void CreateCommandSetSRV(const UINT paraNum)const;

	// ���\�[�X���O������ݒ肷��
	void SetResource(ComPtr<ID3D12Resource1> resource, bool isSrv = false);

	// ���\�[�X�o���A��RT�\��Ԃɂ���
	void BarrierToRT();
	// ���\�[�X�o���A���X���b�v�`�F�C���\���\��Ԃɂ���
	void BarrierToPresent();

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

	//
	bool CreateSRV();

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

	// �e�N�X�`�����\���̎擾
	const D3D12_RESOURCE_DESC& GetInfo() { return m_desc; }

	// SRV�����邩�H
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
	// �f�[�^
	//
	//================================================
	std::string				m_filepath;		// �t�@�C����(LoadTexture�����ꍇ�̂�)
	ComPtr<ID3D12Resource1>	m_texture2D = nullptr;// �e�N�X�`�����\�[�X
	D3D12_RESOURCE_DESC		m_desc = {};	// ���\�[�X���\����

	//-------------------------
	// SRV
	//-------------------------
	// SRV�쐬�p�\����
	D3D12_SHADER_RESOURCE_VIEW_DESC m_srvDesc = {};
	// �n���h��
	//	12�ł́A�f�B�X�N���v�^�q�[�v��View���쐬���A�g���Ƃ���
	//	�f�B�X�N���v�^�q�[�v�ɍ쐬�����ꏊ�̃n���h�����g�p����
	RgHeapHandle m_srv;
};


// 
class RgRenderTargetViews
{
public:

	// RTV���쐬����
	bool CreateRTVs(const std::vector<ComPtr<ID3D12Resource1>>& resources);

	// RTV Descriptor Handle �擾
	const D3D12_CPU_DESCRIPTOR_HANDLE GetRTV_Desc_Handle()const {
		return m_heapRtv->GetCPUDescriptorHandleForHeapStart();
	}

	// RTV�����擾
	const UINT GetRTCount()const { return m_multiRTCount; }

private:
	// �f�B�X�N���v�^�q�[�v�̍쐬
	bool CreateDescriptorHeap();

private:
	UINT m_multiRTCount = 1;				// RTV�̐�
	ComPtr<ID3D12DescriptorHeap> m_heapRtv;	// RTV�p�f�B�X�N���v�^�q�[�v
	UINT m_rtvDescriptorOffset = 0;			// RTV�̈�̑傫��
};


class RgRTChanger
{
public:

	// �ݒ�֌W
	// RTV��ݒ�
	void SetRTV(RgRenderTargetViews* rtv);
	// DSV��ݒ�
	void SetDSV(const D3D12_CPU_DESCRIPTOR_HANDLE& dsv);

	// 
	void CreateCommand_SetRtvDsv(const ComPtr<ID3D12GraphicsCommandList>& command);

private:
	D3D12_CPU_DESCRIPTOR_HANDLE m_depthHandle = {};
	RgRenderTargetViews* m_rtvs = nullptr;
};