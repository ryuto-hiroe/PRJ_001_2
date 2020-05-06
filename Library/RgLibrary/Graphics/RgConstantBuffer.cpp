
#include "RgLibrary.h"
#include "RgConstantBuffer.h"

bool RgUniqueConstantBuffer::CreateConstantBuffer()
{
	HRESULT hr;

	m_numAssigndPos = 0;

	// バッファのサイズ
	m_cbSize = m_bufferCount * m_maxConstantBufferBlocks;
	// バッファの作成
	m_cb = Rg::CreateBuffer(m_cbSize, nullptr);

	// マップしておく
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
//		// エラー
//	}
//
//	int top = m_numAssigndPos;
//	m_numAssigndPos += top;
//
//	// 書き込み
//	memcpy(mappedConstantBuffer + top, src, size);
//
//	//
//	D3D12_CONSTANT_BUFFER_VIEW_DESC cbDesc{};
//	cbDesc.BufferLocation	= m_cb->GetGPUVirtualAddress() + top * 0x100;
//	cbDesc.SizeInBytes		= size;
//
//
//
//	// コンスタントバッファビューの作成
//	return RGHEAPMGR.CreateCBV(&cbDesc);
//}