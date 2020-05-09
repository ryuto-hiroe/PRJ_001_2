
#include "RgLibrary.h"
#include "Graphics/RgTexture.h"

bool RgTexture2D::LoadTexture(const std::string& filename)
{
	HRESULT hr;

	m_filepath = filename;

	auto wString = RgConvertStringToWstring(m_filepath);

	DirectX::ScratchImage image;
	DirectX::TexMetadata mate;

	bool IsSuccess = false;

	//=============================
	// �e�N�X�`���ǂݍ���
	//=============================
	// WIC�摜�ǂݍ���
	hr = DirectX::LoadFromWICFile(wString.c_str(), DirectX::WIC_FLAGS_ALL_FRAMES, &mate, image);
	if (SUCCEEDED(hr)) {	// �ǂݍ��ݐ���
		IsSuccess = true;
	} 

	// TGA�摜�ǂݍ���
	if (IsSuccess == false) {
		hr = DirectX::LoadFromTGAFile(wString.c_str(), &mate, image);
		if (SUCCEEDED(hr)) {	// �ǂݍ��ݐ���
			IsSuccess = true;
		}
	}

	// HDR�摜�ǂݍ���
	if (IsSuccess == false) {
		hr = DirectX::LoadFromHDRFile(wString.c_str(), &mate, image);
		if (SUCCEEDED(hr)) {	// �ǂݍ��ݐ���
			IsSuccess = true;
		}
	}

	if (IsSuccess == false) {	// �ǂݍ��ݎ��s
		Release();
		return false;
	}

	// �e�N�X�`�����g�p�\�ɂ���
	if (SetUpTexture(image, mate) == false) {
		Release();
		return false;
	}

	return true;
}

bool RgTexture2D::LoadTextureFromMemory(const std::vector<char>& imageData)
{
	HRESULT hr;

	DirectX::ScratchImage image;
	DirectX::TexMetadata mate;

	hr = LoadFromWICMemory(imageData.data(), imageData.size(), 0, &mate, image);
	if (FAILED(hr)) {
		Release();
		return false;
	}

	// �e�N�X�`�����g�p�\�ɂ���
	if (SetUpTexture(image, mate) == false) {
		Release();
		return false;
	}

	return true;
}

bool RgTexture2D::CreateTexAndSRV(const int w, const int h, DXGI_FORMAT format)
{
	// �e�N�X�`�����\�[�X�쐬
	auto desc = CD3DX12_RESOURCE_DESC::Tex2D(
		format, w, h,
		1, 1, 1, 0, // ArraySize, MipLevel, SampleCount, SampleQuality
		D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET);

	m_format = format;

	// �e�N�X�`�����\�[�X�쐬
	D3D12_CLEAR_VALUE clearValue = {};
	clearValue.Format = m_format;
	RGD3D.GetDevice()->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
		D3D12_HEAP_FLAG_NONE,
		&desc,
		D3D12_RESOURCE_STATE_RENDER_TARGET,
		&clearValue,
		IID_PPV_ARGS(&m_texture2D)
	);

	// SRV�̍쐬
	CreateSRV();

	return true;
}

bool RgTexture2D::CreateSRV()
{
	// SRV�̍쐬
	m_srvDesc.Texture2D.MipLevels = 1;
	m_srvDesc.Format = m_format;
	m_srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	m_srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	m_srv = RGHEAPMGR.CreateSRV(m_texture2D, &m_srvDesc);

	return true;
}


bool RgTexture2D::SetUpTexture(
	const DirectX::ScratchImage& image,
	const DirectX::TexMetadata& mate)
{
	HRESULT hr;

	//=============================
	// �e�N�X�`���̍쐬
	//=============================
	ComPtr<ID3D12Resource> tex;
	std::vector<D3D12_SUBRESOURCE_DATA> subres;
	hr = DirectX::CreateTexture(RGD3D.GetDevice().Get(), mate, &tex);
	if (FAILED(hr)) {	// �ǂݍ��ݎ��s
		return false;
	}

	//=============================
	// �A�b�v���[�h�q�[�v�p����
	//=============================
	hr = PrepareUpload(RGD3D.GetDevice().Get(), image.GetImages(), image.GetImageCount(), mate, subres);
	if (FAILED(hr)) {	// �ǂݍ��ݎ��s
		return false;
	}
	const auto totalBytes = GetRequiredIntermediateSize(tex.Get(), 0, UINT(subres.size()));

	//=============================
	// �X�e�[�W���O�o�b�t�@����
	//=============================
	ComPtr<ID3D12Resource> uploadHeap = nullptr;
	hr = RGD3D.GetDevice()->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(totalBytes),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&uploadHeap)
	);
	if (FAILED(hr)) {	// �ǂݍ��ݎ��s
		return false;
	}

	//=============================
	// �]������
	//=============================
	RGD3D.WaitPreviousFrame();
	// �]��
	RGD3D.m_texCommandList->Reset(
		RGD3D.m_commandAllocators2[RGD3D.m_frameIndex].Get(), nullptr);
	UpdateSubresources(
		RGD3D.m_texCommandList.Get(),
		tex.Get(),
		uploadHeap.Get(),
		0, 0,
		uint32_t(subres.size()),
		subres.data()
	);
	// ���\�[�X�o���A�̃Z�b�g
	auto barrierTex = CD3DX12_RESOURCE_BARRIER::Transition(
		tex.Get(),
		D3D12_RESOURCE_STATE_COPY_DEST,
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE
	);
	RGD3D.m_texCommandList->ResourceBarrier(1, &barrierTex);
	// �R�}���h�̎��s
	RGD3D.m_texCommandList->Close();
	ID3D12CommandList* cmds[] = { RGD3D.m_texCommandList.Get() };
	RGD3D.m_commandQueue->ExecuteCommandLists(1, cmds);
	RGD3D.m_frameFenceValues[RGD3D.m_frameIndex]++;
	RGD3D.WaitGPU();

	tex.As(&m_texture2D);

	//=============================
	// SRV�r���[�̍쐬
	//=============================
	{
		m_srvDesc.Texture2D.MipLevels = 1;
		m_srvDesc.Format = mate.format;
		m_srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
		m_srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;

		m_srv = RGHEAPMGR.CreateSRV(m_texture2D, &m_srvDesc);
	}

	return true;
}

void RgTexture2D::Release()
{
	m_filepath = "";
	m_srvDesc = {};
	RGHEAPMGR.Free(m_srv.index);
}

void RgTexture2D::CreateCommandSetSRV(const ComPtr<ID3D12GraphicsCommandList>& command, const UINT paraNum)const
{
	command->SetGraphicsRootDescriptorTable(paraNum, m_srv.handle);
}





// �f�B�X�N���v�^�q�[�v�̍쐬
bool RgRenderTargets::CreateDescriptorHeap()
{
	HRESULT hr;
	D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc{
		D3D12_DESCRIPTOR_HEAP_TYPE_RTV,
		m_multiRTCount, // �����_�[�^�[�Q�b�g��
		D3D12_DESCRIPTOR_HEAP_FLAG_NONE,
		0
	};
	hr = RGD3D.GetDevice()->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&m_heapRtv));
	if (FAILED(hr)) {
		return false;
	}
	m_rtvDescriptorSize = RGD3D.GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

	return true;
}

void RgRenderTargets::CreateCommand_SetRTV_All(ComPtr<ID3D12GraphicsCommandList>& comList)
{
	for (UINT i = 0; i < m_multiRTCount; i++)
	{
		auto barrier = CD3DX12_RESOURCE_BARRIER::Transition(
			m_rt[i]->GetResource().Get(),
			D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
			D3D12_RESOURCE_STATE_RENDER_TARGET
		);
		RGD3D.m_texCommandList->ResourceBarrier(1, &barrier);
	}
	comList->OMSetRenderTargets(
		m_multiRTCount, &m_rtvHandle, false, nullptr);
}

void RgRenderTargets::CreateCommand_SetRTV(
	const UINT num, ComPtr<ID3D12GraphicsCommandList>& comList)
{
	if (num >= m_multiRTCount)return;

	auto barrier = CD3DX12_RESOURCE_BARRIER::Transition(
		m_rt[num]->GetResource().Get(),
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
		D3D12_RESOURCE_STATE_RENDER_TARGET
	);
	RGD3D.m_texCommandList->ResourceBarrier(1, &barrier);

	CD3DX12_CPU_DESCRIPTOR_HANDLE workHandle = m_rtvHandle;
	workHandle.Offset(num, m_rtvDescriptorSize);
	comList->OMSetRenderTargets(
		1, &workHandle, false, nullptr);
}

void RgRenderTargets::CreateCommand_ResetRTV_All(
	ComPtr<ID3D12GraphicsCommandList>& comList)
{
	for (UINT i = 0; i < m_multiRTCount; i++)
	{
		auto barrier = CD3DX12_RESOURCE_BARRIER::Transition(
			m_rt[i]->GetResource().Get(),
			D3D12_RESOURCE_STATE_RENDER_TARGET,
			D3D12_RESOURCE_STATE_PRESENT
		);
		RGD3D.m_texCommandList->ResourceBarrier(1, &barrier);
	}
}
void RgRenderTargets::CreateCommand_ResetRTV(
	const UINT num, ComPtr<ID3D12GraphicsCommandList>& comList)
{
	if (num >= m_multiRTCount)return;
	// �����_�[�^�[�Q�b�g����X���b�v�`�F�C���\���\��
	auto barrierToPresent = CD3DX12_RESOURCE_BARRIER::Transition(
		m_rt[num]->GetResource().Get(),
		D3D12_RESOURCE_STATE_RENDER_TARGET,
		D3D12_RESOURCE_STATE_PRESENT
	);
	RGD3D.m_texCommandList->ResourceBarrier(1, &barrierToPresent);
}

void RgRenderTargets::CreateCommand_SetSRV(
	const UINT num, const ComPtr<ID3D12GraphicsCommandList>& command, const UINT paraNum)const
{
	if (num >= m_multiRTCount)return;
	auto barrier = CD3DX12_RESOURCE_BARRIER::Transition(
		m_rt[num]->GetResource().Get(),
		D3D12_RESOURCE_STATE_RENDER_TARGET,
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE
	);
	command->ResourceBarrier(1, &barrier);

	m_rt[num]->CreateCommandSetSRV(command, paraNum);
}

void RgRenderTargets::ClearRT_All(
	RgCommandList& comList, const RgVec4 clearColor)
{
	for (UINT i = 0; i < m_multiRTCount; i++)
	{
		CD3DX12_CPU_DESCRIPTOR_HANDLE rtv(
			m_heapRtv->GetCPUDescriptorHandleForHeapStart(),
			i, m_rtvDescriptorSize);
		// �J���[�o�b�t�@(�����_�[�^�[�Q�b�g�r���[)�̃N���A
		comList.GetCommandList()->ClearRenderTargetView(rtv, &clearColor.x, 0, nullptr);
	}
}
void RgRenderTargets::ClearRt(
	const UINT num, RgCommandList& comList, const RgVec4 clearColor)
{
	if (num >= m_multiRTCount)return;
	CD3DX12_CPU_DESCRIPTOR_HANDLE rtv(
		m_heapRtv->GetCPUDescriptorHandleForHeapStart(),
		num, m_rtvDescriptorSize);
	// �J���[�o�b�t�@(�����_�[�^�[�Q�b�g�r���[)�̃N���A
	comList.GetCommandList()->ClearRenderTargetView(rtv, &clearColor.x, 0, nullptr);
}

void RgRenderTargets::SetUp(const UINT rtNum, const UINT w, const UINT h)
{
	m_IsSetUp = true;

	m_multiRTCount = rtNum;
	m_rtW = w;
	m_rtH = h;

	// RTV�p�f�B�X�N���v�^�q�[�v�쐬
	CreateDescriptorHeap();
	// �f�B�X�N���v�^�q�[�v�̐擪�̃n���h����ێ�����
	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(
		m_heapRtv->GetCPUDescriptorHandleForHeapStart());
	m_rtvHandle = rtvHandle;	
}

bool RgRenderTargets::CreateRT_All(DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM)
{
	if (m_IsSetUp == false)return false;

	//==========================
	// �e�N�X�`���쐬
	//==========================
	for (int i = 0; i < m_multiRTCount; i++)
	{
		auto rt = std::make_shared<RgTexture2D>();
		rt->CreateTexAndSRV(m_rtW, m_rtH, format);
		m_rt.push_back(rt);
	}
	//==========================
	// �e�N�X�`����RTV���쐬����
	//==========================
	CD3DX12_CPU_DESCRIPTOR_HANDLE workHandle = m_rtvHandle;
	for (int i = 0; i < m_multiRTCount; i++)
	{
		RGD3D.GetDevice()->CreateRenderTargetView(m_rt[i]->GetResource().Get(), nullptr, workHandle);
		workHandle.Offset(1, m_rtvDescriptorSize);
	}

	return true;
}

bool RgRenderTargets::CreateRT(
	DXGI_FORMAT format,
	const UINT w = 0, const UINT h = 0)
{
	if (m_IsSetUp == false)return false;
	if (m_rt.size() >= m_multiRTCount)
		return false;

	// RT�T�C�Y���v�Z
	UINT useW = w, useH = h;
	if (useW == 0)useW = m_rtW;
	if (useH == 0)useH = m_rtH;

	// �e�N�X�`���쐬
	auto rt = std::make_shared<RgTexture2D>();
	rt->CreateTexAndSRV(useW, useH, format);
	m_rt.push_back(rt);

	// RTV�쐬
	int now = (int)m_rt.size() - 1;
	CD3DX12_CPU_DESCRIPTOR_HANDLE workHandle = m_rtvHandle;
	workHandle.Offset((UINT)now, m_rtvDescriptorSize);
	RGD3D.GetDevice()->CreateRenderTargetView(m_rt.back()->GetResource().Get(), nullptr, workHandle);

	return true;
}