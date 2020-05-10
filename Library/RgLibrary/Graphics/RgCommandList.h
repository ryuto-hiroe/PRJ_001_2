#pragma once

//=============================================
//
// 1�̃R�}���h���X�g�N���X
//	�EcommandList��commandAllocator�������Ă���
//
//=============================================
class RgCommandList : public std::enable_shared_from_this<RgCommandList>
{
public:

	// �쐬
	// �Enum�@�E�E�E�@�A���P�[�^�̍쐬���B0���w�肷��ƃt���[�������쐬
	bool Create(const UINT num = 0);

	// �R�}���h�̊J�n
	void Begin();
	// �R�}���h�̏I��
	void End();

	//========================
	//
	// �擾�֌W
	//
	//========================
	// �R�}���h�A���P�[�^�̎擾
	const std::vector<ComPtr<ID3D12CommandAllocator>>& GetAllocators(){ 
		return m_commandAllocators; 
	}
	// �R�}���h���X�g�̎擾
	ComPtr<ID3D12GraphicsCommandList>& GetCommandList() {
		return m_commandList;
	}


private:
	// �`��R�}���h��~����N���X
	//	�E����̓t���[�������p������
	std::vector<ComPtr<ID3D12CommandAllocator>> m_commandAllocators;
	// �`��R�}���h���쐬����N���X
	ComPtr<ID3D12GraphicsCommandList> m_commandList;
};


//=============================================
//
// �R�}���h���X�g���Ǘ�����N���X
//	�ERgCommandList	
//
//=============================================
class RgCommandListManager
{
public:

	// ������
	bool Init();


	//========================
	//
	// �擾�֌W
	//
	//========================
	// ���f���`��p�R�}���h���X�g
	std::vector<RgCommandList>& GetModelCommandList() {
		return m_drawModelCommandList;
	}

	// ���[�N�p�R�}���h���X�g
	RgCommandList& GetWorkCommandList() {
		return m_workCommandList;
	}


	ComPtr<ID3D12GraphicsCommandList> m_drawCommandList;

private:

	// ���[�N�p�R�}���h���X�g
	RgCommandList m_workCommandList;

	// ���f���`��R�}���h�����p
	std::vector<RgCommandList> m_drawModelCommandList;


	//========================
	// �V���O���g���p�^�[��
	//========================
private:
	//
	RgCommandListManager() {}

public:

	static RgCommandListManager& GetInstance() {
		static RgCommandListManager inc;
		return inc;
	}
};
#define COMLIST RgCommandListManager::GetInstance()