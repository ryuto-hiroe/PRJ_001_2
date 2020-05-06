#pragma once

namespace Rg
{
	// �ȒP�Ƀo�b�t�@���쐬����֐�
	static ComPtr<ID3D12Resource1> CreateBuffer(UINT size, const void* src);
};


//====================================================
//
// ���I�ɒ��_�o�b�t�@���쐬����N���X
//
//====================================================
template<class DataType>
class RgDynamicVertexBuffer
{
public:

	//===================================
	//
	// �擾�֌W
	//
	//===================================
	// �o�b�t�@�̍\���̂��擾(�ύX�\)
	std::vector<DataType>& GetWork() {
		//m_changeData = true;
		return m_data;
	}

	// �o�b�t�@�̍\���̂��擾(�ύX�s��)
	//const DataType& GetWork_Read() const { return m_dataType; }

	//===================================
	//
	// �쐬�֌W
	//
	//===================================
	// �o�b�t�@�쐬
	bool Create(const UINT dataNum);

	// CBV��ݒ肷��R�}���h���쐬
	void CreateCommandSetCBV(ComPtr<ID3D12GraphicsCommandList>& command)const;

	// �f�[�^��������
	void WriteData();

private:
	// �萔�o�b�t�@�̃f�[�^
	std::vector<DataType> m_data;

	// �萔�o�b�t�@�������ݐ�
	DataType* m_mapped;

	// �o�b�t�@�̃T�C�Y
	UINT m_bufferSize = 0;

	// �S�̂̑傫��
	UINT m_totalSize = 0;

	// �o�b�t�@
	ComPtr<ID3D12Resource> m_buffer;

	D3D12_VERTEX_BUFFER_VIEW m_bufferView;

	// ���_�o�b�t�@�p�������m�ې�
	const UINT m_MaxNum = 256;

	// ���݂̎g�p�Ԓn
	UINT m_useIndex = 0;
};



//====================================================
//
// �P�t���[���ɂP��ȏ�萔�o�b�t�@�̓��e��ύX���Ȃ����m�p
//
//====================================================
template<class DataType>
class RgStaticConstantBuffer
{
public:

	//===================================
	//
	// �擾�֌W
	//
	//===================================
	// �o�b�t�@�̍\���̂��擾(�ύX�\)
	DataType& GetWork() {
		m_changeData = true;
		return m_dataType;
	}

	// �o�b�t�@�̍\���̂��擾(�ύX�s��)
	const DataType& GetWork_Read() const { return m_dataType; }

	//===================================
	//
	// �쐬�֌W
	//
	//===================================
	// �o�b�t�@�쐬
	bool Create();

	// CBV��ݒ肷��R�}���h���쐬
	void CreateCommandSetCBV(ComPtr<ID3D12GraphicsCommandList>& command, UINT paraNum)const;
	
	// �f�[�^��������
	void WriteData();

	// ���ׂẴo�b�t�@�ɏ�������
	void WriteData_AllBuffer();

private:

	// �萔�o�b�t�@�̃f�[�^
	DataType m_dataType;

	// �萔�o�b�t�@�������ݐ�
	//std::vector<DataType*> m_mapped;
	DataType* m_mapped;

	// �o�b�t�@
	//std::vector<ComPtr<ID3D12Resource>> m_buffer;
	ComPtr<ID3D12Resource> m_buffer;

	//CBV
	//std::vector<RgHeapHandle> m_cbv;
	RgHeapHandle m_cbv;

	// �萔�o�b�t�@�̃T�C�Y
	UINT m_bufferSize = 0;

	// ���̃t���O��true�̎������f�[�^����������
	bool m_changeData = false;
};

//====================================================
//
// ���I��CBV���쐬����萔�o�b�t�@�N���X
//
//====================================================
template<class DataType>
class RgDynamicConstantBuffer
{
public:

	//===================================
	//
	// �擾�֌W
	//
	//===================================
	// �o�b�t�@�̍\���̂��擾(�ύX�\)
	DataType& GetWork() {
		m_changeData = true;
		return m_dataType;
	}

	// �o�b�t�@�̍\���̂��擾(�ύX�s��)
	const DataType& GetWork_Read() const { return m_dataType; }


	//===================================
	//
	// �쐬�֌W
	//
	//===================================
	// �o�b�t�@�쐬
	bool Create();

	// CBV��ݒ肷��R�}���h���쐬
	void CreateCommandSetCBV(ComPtr<ID3D12GraphicsCommandList>& command, UINT paraNum);



	// �f�[�^��������
	void WriteData();

private:
	// �萔�o�b�t�@�̍쐬��
	const UINT m_MaxCBNum = 256;

	// �o�b�t�@
	ComPtr<ID3D12Resource> m_buffer;

	//CBV
	RgHeapHandle m_cbv;
	// �萔�o�b�t�@�̃f�[�^
	DataType m_dataType;
	// �萔�o�b�t�@�̃T�C�Y
	UINT m_bufferSize = 0;

	// �������ݐ�
	const UINT m_dataSize = 0x100; //256ppe
	struct { char buf[0x100]; }*mapped;

//	DataType* mapped;
	//
	UINT m_Sride = 0;
	// ���ݎg�p�ł���ꏊ
	UINT m_nextUsePos = 0;

	// �g�p����CB��DH��Index���L��
	std::unordered_map<UINT, UINT> m_saveIndicesInUse;

	// ���̃t���O��true�̎������f�[�^����������
	bool m_changeData = false;
};
#include "RgBuffer.inl"