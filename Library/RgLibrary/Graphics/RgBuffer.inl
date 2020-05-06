#pragma once

ComPtr<ID3D12Resource1> Rg::CreateBuffer(UINT size, const void* src)
{
	HRESULT hr;
	ComPtr<ID3D12Resource1> buffer;
	hr = RGD3D.GetDevice()->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(size),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&buffer)
	);

	// 初期データの指定があるときにはコピーする
	if (SUCCEEDED(hr) && src != nullptr)
	{
		void* mapped;
		CD3DX12_RANGE range(0, 0);
		hr = buffer->Map(0, &range, &mapped);
		if (SUCCEEDED(hr))
		{
			memcpy(mapped, src, size);
			buffer->Unmap(0, nullptr);
		}
	}

	return buffer;
}

//=====================================================
// 動的頂点バッファ
//=====================================================
template<class DataType>
inline bool RgDynamicVertexBuffer<DataType>::Create(const UINT dataNum)
{
	m_data.resize(dataNum);

	m_bufferSize = sizeof(DataType) * dataNum;
	m_totalSize = m_bufferSize * m_MaxNum;

	m_buffer = Rg::CreateBuffer(m_totalSize, nullptr);

	CD3DX12_RANGE range(0, 0);
	m_buffer->Map(0, &range, (void**)&m_mapped);

	return true;
}

template<class DataType>
inline void RgDynamicVertexBuffer<DataType>::WriteData()
{
	// 
	if (m_useIndex >= m_MaxNum) {
		m_useIndex = 0;
	}

	// データ書き込み
	memcpy(m_mapped + m_useIndex, &m_data[0], m_bufferSize);

	// 頂点バッファビューの作成
	m_bufferView.BufferLocation = m_buffer->GetGPUVirtualAddress() + (m_useIndex * m_bufferSize);
	m_bufferView.SizeInBytes = m_bufferSize;
	m_bufferView.StrideInBytes = sizeof(DataType);
}

template<class DataType>
inline void RgDynamicVertexBuffer<DataType>::CreateCommandSetCBV(
	ComPtr<ID3D12GraphicsCommandList>& command)const
{
	command->IASetVertexBuffers(0, 1, &m_bufferView);
}

//=====================================================
// 静的コンスタントバッファ
//=====================================================
template<class DataType>
inline bool RgStaticConstantBuffer<DataType>::Create()
{
	// 256区切りにする
	m_bufferSize = sizeof(DataType) + 255 & ~255;

	// バッファの作成
	m_buffer = Rg::CreateBuffer(m_bufferSize, nullptr);

	// CBVの作成
	D3D12_CONSTANT_BUFFER_VIEW_DESC cbDesc{};
	cbDesc.BufferLocation = m_buffer->GetGPUVirtualAddress();
	cbDesc.SizeInBytes = m_bufferSize;
	m_cbv = RGHEAPMGR.CreateCBV(&cbDesc);

	// マップする
	// DirectX12からはMapしっぱなしでもOK。
	CD3DX12_RANGE range(0, 0);
	m_buffer->Map(0, &range, (void**)&m_mapped);

	return true;
}

template<class DataType>
inline void RgStaticConstantBuffer<DataType>::WriteData()
{
	if (m_changeData)
	{
		//memcpy(m_mapped[RGD3D.m_frameIndex], &m_dataType, m_bufferSize);
		memcpy(m_mapped, &m_dataType, m_bufferSize);

		m_changeData = false;
	}
}

template<class DataType>
inline void RgStaticConstantBuffer<DataType>::WriteData_AllBuffer()
{
	if (m_changeData)
	{
		//for (UINT i = 0; i < RGD3D.FrameBufferCount; i++)
		//{
		//	memcpy(m_mapped[i], &m_dataType, m_bufferSize);
		//}
		memcpy(m_mapped, &m_dataType, m_bufferSize);
		m_changeData = false;
	}
}

template<class DataType>
inline void RgStaticConstantBuffer<DataType>::CreateCommandSetCBV(
	ComPtr<ID3D12GraphicsCommandList>& command, UINT paraNum)const
{
	//command->SetGraphicsRootDescriptorTable(paraNum, m_cbv[RGD3D.m_frameIndex].handle);
	command->SetGraphicsRootDescriptorTable(paraNum, m_cbv.handle);
}



//=====================================================
// 動的コンスタントバッファ
//=====================================================
template<class DataType>
inline bool RgDynamicConstantBuffer<DataType>::Create()
{
	HRESULT hr;

	m_Sride = sizeof(DataType) + 255 & ~255;
	//
	m_bufferSize = m_dataSize * m_MaxCBNum;

	// バッファの作成
	m_buffer = Rg::CreateBuffer(m_bufferSize, nullptr);
	
	// マップしておく
	CD3DX12_RANGE range(0, 0);
	hr = m_buffer->Map(0, &range, (void**)&mapped);
	if (FAILED(hr)) {
		return false;
	}

	return true;
}

template<class DataType>
inline void RgDynamicConstantBuffer<DataType>::WriteData()
{
	if (m_changeData)
	{
		// 
		int stepNum = m_Sride / m_dataSize;
		//
		if (m_nextUsePos + stepNum > m_MaxCBNum) {
			m_nextUsePos = 0;
		}
		
		auto top = m_nextUsePos;
		m_nextUsePos += stepNum;

		memcpy(mapped + top, &m_dataType, sizeof(DataType));

		// CBVを作成
		// ディスクリプタヒープがあるかを調べる
		auto it = m_saveIndicesInUse.find(top);
		if (it != m_saveIndicesInUse.end())
		{	// ディスクリプタヒープがあった場合はそれを解放する
			RGHEAPMGR.Free(m_saveIndicesInUse[top]);
			//m_saveIndicesInUse.erase(top);
		}
		// 新たにCBVを作成する
		D3D12_CONSTANT_BUFFER_VIEW_DESC cbDesc{};
		cbDesc.BufferLocation = m_buffer->GetGPUVirtualAddress() + top * m_dataSize;
		cbDesc.SizeInBytes = m_Sride;
		m_cbv = RGHEAPMGR.CreateCBV(&cbDesc);
		// 新たに使用中になったものを記憶する
		m_saveIndicesInUse[top] = m_cbv.index;

		m_changeData = false;
	}
}

template<class DataType>
void RgDynamicConstantBuffer<DataType>::CreateCommandSetCBV(
	ComPtr<ID3D12GraphicsCommandList>& command, UINT paraNum)
{
	command->SetGraphicsRootDescriptorTable(paraNum, m_cbv.handle);
}