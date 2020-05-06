#pragma once





//===============================================================
//
// �R���X�^���g�o�b�t�@���Ǘ�����N���X
//	�E��傫�ȃR���X�^���g�o�b�t�@���쐬����
//
//===============================================================
class RgUniqueConstantBuffer
{
public:

	// �R���X�^���g�o�b�t�@�쐬
	bool CreateConstantBuffer();

	// �f�[�^��������
	// �萔�o�b�t�@�\���̃A�h���X
	// �萔�o�b�t�@�\���̃T�C�Y
	RgHeapHandle& WiteData(void* src, const UINT size);

	// �O�`�悪�I�������A�N�Z�b�T�[���͂��߂ɖ߂�
	void ResetAccesser() {
		m_numAssigndPos = 0;
	}


private:

	// �R���X�^���g�o�b�t�@�̌�
	const UINT m_bufferCount = 1000;

	// ��̃R���X�^���g�o�b�t�@�̍ő�T�C�Y
	const UINT m_maxConstantBufferBlocks = 0x100;

	// �R���X�^���g�o�b�t�@�̃T�C�Y
	UINT m_cbSize = 0;

	// ���݃A�N�Z�X�ł���ꏊ
	UINT m_numAssigndPos = 0;

	// �B��̃R���X�^���g�o�b�t�@
	ComPtr<ID3D12Resource1> m_cb = nullptr;
	// �������ݐ�̃A�h���X
	struct {
		char buf[0x100];
	}
	*mappedConstantBuffer = nullptr;

	// 


	//====================================
	// �V���O���g���p�^�[��
	//====================================
private:
	//
	RgUniqueConstantBuffer(){}

public:
	//
	static RgUniqueConstantBuffer& GetInstance()
	{
		static RgUniqueConstantBuffer instance;
		return instance;
	}
};
#define UCB RgUniqueConstantBuffer::GetInstance()