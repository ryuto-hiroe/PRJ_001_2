
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
	// テクスチャ読み込み
	//=============================
	// WIC画像読み込み
	hr = DirectX::LoadFromWICFile(wString.c_str(), DirectX::WIC_FLAGS_ALL_FRAMES, &mate, image);
	if (SUCCEEDED(hr)) {	// 読み込み成功
		IsSuccess = true;
	} 

	// TGA画像読み込み
	if (IsSuccess == false) {
		hr = DirectX::LoadFromTGAFile(wString.c_str(), &mate, image);
		if (SUCCEEDED(hr)) {	// 読み込み成功
			IsSuccess = true;
		}
	}

	// HDR画像読み込み
	if (IsSuccess == false) {
		hr = DirectX::LoadFromHDRFile(wString.c_str(), &mate, image);
		if (SUCCEEDED(hr)) {	// 読み込み成功
			IsSuccess = true;
		}
	}

	if (IsSuccess == false) {	// 読み込み失敗
		Release();
		return false;
	}

	// テクスチャを使用可能にする
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

	// テクスチャを使用可能にする
	if (SetUpTexture(image, mate) == false) {
		Release();
		return false;
	}

	return true;
}

bool RgTexture2D::CreateTexAndSRV(const int w, const int h, DXGI_FORMAT format)
{
	Release();

	// テクスチャリソース作成
	auto desc = CD3DX12_RESOURCE_DESC::Tex2D(
		format, w, h,
		1, 1, 1, 0, // ArraySize, MipLevel, SampleCount, SampleQuality
		D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET);

	m_desc = desc;

	// テクスチャリソース作成
	D3D12_CLEAR_VALUE clearValue = {};
	clearValue.Format = m_desc.Format;
	RGD3D.GetDevice()->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
		D3D12_HEAP_FLAG_NONE,
		&desc,
		D3D12_RESOURCE_STATE_RENDER_TARGET,
		&clearValue,
		IID_PPV_ARGS(&m_texture2D)
	);

	// SRVの作成
	CreateSRV();

	return true;
}

void RgTexture2D::SetResource(ComPtr<ID3D12Resource1> resource, bool isSrv)
{
	Release();

	m_texture2D = resource;
	m_desc = m_texture2D->GetDesc();

	// SRVの作成
	if (isSrv){
		CreateSRV();
	}
}

bool RgTexture2D::CreateSRV()
{
	// SRVの作成
	m_srvDesc.Texture2D.MipLevels = 1;
	m_srvDesc.Format = m_desc.Format;
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
	// テクスチャの作成
	//=============================
	ComPtr<ID3D12Resource> tex; 
	std::vector<D3D12_SUBRESOURCE_DATA> subres;
	hr = DirectX::CreateTexture(RGD3D.GetDevice().Get(), mate, &tex);
	if (FAILED(hr)) {	// 読み込み失敗
		return false;
	}

	//=============================
	// アップロードヒープ用準備
	//=============================
	hr = PrepareUpload(RGD3D.GetDevice().Get(), image.GetImages(), image.GetImageCount(), mate, subres);
	if (FAILED(hr)) {	// 読み込み失敗
		return false;
	}
	const auto totalBytes = GetRequiredIntermediateSize(tex.Get(), 0, UINT(subres.size()));

	//=============================
	// ステージングバッファ準備
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
	if (FAILED(hr)) {	// 読み込み失敗
		return false;
	}

	//=============================
	// 転送処理
	//=============================
	RGD3D.WaitPreviousFrame();
	// 転送
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
	// リソースバリアのセット
	auto barrierTex = CD3DX12_RESOURCE_BARRIER::Transition(
		tex.Get(),
		D3D12_RESOURCE_STATE_COPY_DEST,
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE
	);
	RGD3D.m_texCommandList->ResourceBarrier(1, &barrierTex);
	// コマンドの実行
	RGD3D.m_texCommandList->Close();
	ID3D12CommandList* cmds[] = { RGD3D.m_texCommandList.Get() };
	RGD3D.m_commandQueue->ExecuteCommandLists(1, cmds);
	RGD3D.m_frameFenceValues[RGD3D.m_frameIndex]++;
	RGD3D.WaitGPU();

	tex.As(&m_texture2D);

	//=============================
	// SRVビューの作成
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
	m_desc = {};
	m_srvDesc = {};
	if (m_srv.index >= 0)RGHEAPMGR.Free(m_srv.index);
}

void RgTexture2D::CreateCommandSetSRV(const UINT paraNum)const
{
	RGD3D.GetCL()->SetGraphicsRootDescriptorTable(paraNum, m_srv.handle);
}

void RgTexture2D::BarrierToRT()
{
	// スワップチェイン表示可能からレンダーターゲット描画可能へ
	auto barrierToRT = CD3DX12_RESOURCE_BARRIER::Transition(
		m_texture2D.Get(),
		D3D12_RESOURCE_STATE_PRESENT,
		D3D12_RESOURCE_STATE_RENDER_TARGET
	);
	RGD3D.GetCL()->ResourceBarrier(1, &barrierToRT);
	//command->ResourceBarrier(1, &barrierToRT);
}
void RgTexture2D::BarrierToPresent()
{
	// レンダーターゲットからスワップチェイン表示可能へ
	auto barrierToPresent = CD3DX12_RESOURCE_BARRIER::Transition(
		m_texture2D.Get(),
		D3D12_RESOURCE_STATE_RENDER_TARGET,
		D3D12_RESOURCE_STATE_PRESENT
	);
	RGD3D.GetCL()->ResourceBarrier(1, &barrierToPresent);
}



bool RgRenderTargetViews::CreateRTVs(
	const std::vector<ComPtr<ID3D12Resource1>>& resources)
{
	m_multiRTCount = (UINT)resources.size();
	if (m_multiRTCount == 0)return false;
	if (CreateDescriptorHeap() == false)return false;

	// RTV作成
	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(
		m_heapRtv->GetCPUDescriptorHandleForHeapStart());
	for (UINT i = 0; i < m_multiRTCount; i++)
	{
		RGD3D.GetDevice()->CreateRenderTargetView(resources[i].Get(), nullptr, rtvHandle);
		rtvHandle.Offset(1, m_rtvDescriptorOffset);
	}

	return true;
}

bool RgRenderTargetViews::CreateDescriptorHeap()
{
	HRESULT hr;
	D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc{
		D3D12_DESCRIPTOR_HEAP_TYPE_RTV,
		m_multiRTCount, // レンダーターゲット数
		D3D12_DESCRIPTOR_HEAP_FLAG_NONE,
		0
	};
	hr = RGD3D.GetDevice()->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&m_heapRtv));
	if (FAILED(hr)) {
		return false;
	}
	m_rtvDescriptorOffset = RGD3D.GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

	return true;
}



void RgRTChanger::SetRTV(RgRenderTargetViews* rtv)
{
	m_rtvs = rtv;
}
void RgRTChanger::SetDSV(const D3D12_CPU_DESCRIPTOR_HANDLE& dsv)
{
	m_depthHandle = dsv;
}
void RgRTChanger::CreateCommand_SetRtvDsv(const ComPtr<ID3D12GraphicsCommandList>& command)
{
	command->OMSetRenderTargets(
		m_rtvs->GetRTCount(),
		&m_rtvs->GetRTV_Desc_Handle(),
		false,
		&m_depthHandle
	);
}