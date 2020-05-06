
#include "RgLibrary.h"
#include "RgConstantBuffer.h"

bool RgUniqueConstantBuffer::CreateConstantBuffer()
{
	HRESULT hr;

	m_numAssigndPos = 0;

	// �o�b�t�@�̃T�C�Y
	m_cbSize = m_bufferCount * m_maxConstantBufferBlocks;
	// �o�b�t�@�̍쐬
	m_cb = Rg::CreateBuffer(m_cbSize, nullptr);

	// �}�b�v���Ă���
	CD3DX12_RANGE range(0, 0);
	hr = m_cb->Map(0, &range, (void**)&mappedConstantBuffer);
	if (FAILED(hr)) {
		return false;
	}

	return true;
}

//RgHeapHandle RgUniqueConstantBuffer::WiteData(void* src, const UINT size)
//{
//	int numRequired = size / 0x100;
//
//	if (m_numAssigndPos + numRequired > m_maxConstantBufferBlocks)
//	{
//		// �G���[
//	}
//
//	int top = m_numAssigndPos;
//	m_numAssigndPos += top;
//
//	// ��������
//	memcpy(mappedConstantBuffer + top, src, size);
//
//	//
//	D3D12_CONSTANT_BUFFER_VIEW_DESC cbDesc{};
//	cbDesc.BufferLocation	= m_cb->GetGPUVirtualAddress() + top * 0x100;
//	cbDesc.SizeInBytes		= size;
//
//
//
//	// �R���X�^���g�o�b�t�@�r���[�̍쐬
//	return RGHEAPMGR.CreateCBV(&cbDesc);
//}