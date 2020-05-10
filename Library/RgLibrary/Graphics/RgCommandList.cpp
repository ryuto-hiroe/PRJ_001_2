#include "RgLibrary.h"
#include "Graphics/RgCommandList.h"

bool RgCommandList::Create(const UINT num)
{
	HRESULT hr;
	//========================
	// �R�}���h�A���P�[�^����
	//========================
	UINT alNum = num;
	if (num == 0) {
		alNum = RGD3D.FrameBufferCount;
	}
	m_commandAllocators.resize(alNum);
	for (UINT i = 0; i < alNum; ++i)
	{
		hr = RGD3D.GetDevice()->CreateCommandAllocator(
			D3D12_COMMAND_LIST_TYPE_DIRECT,
			IID_PPV_ARGS(&m_commandAllocators[i])
		);
		if (FAILED(hr)) {
			return false;
		}
	}
	//========================
	// �R�}���h���X�g����
	//========================
	hr = RGD3D.GetDevice()->CreateCommandList(
		0,
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		m_commandAllocators[0].Get(),
		nullptr,
		IID_PPV_ARGS(&m_commandList)
	);
	if (FAILED(hr)) {
		return false;
	}
	m_commandList->Close();

	return true;
}

void RgCommandList::Begin()
{
	RGD3D.SetUseCL(shared_from_this());
	m_commandAllocators[RGD3D.m_frameIndex]->Reset();
	m_commandList->Reset(m_commandAllocators[RGD3D.m_frameIndex].Get(), nullptr);
}
void RgCommandList::End()
{
	m_commandList->Close();
}

bool RgCommandListManager::Init()
{
	// ���[�N�p�R�}���h���X�g����
	m_workCommandList.Create(1);

	// ���f���`��p�R�}���h���X�g����
	const UINT tNum = 1;
	m_drawModelCommandList.resize(tNum);
	for (int i = 0; i < tNum; i++) {
		m_drawModelCommandList[i].Create();
	}

	return true;
}