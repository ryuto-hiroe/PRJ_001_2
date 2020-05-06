
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
	// テクスチャリソース作成
	auto desc = CD3DX12_RESOURCE_DESC::Tex2D(
		format, w, h,
		1, 1, 1, 0, // ArraySize, MipLevel, SampleCount, SampleQuality
		D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET);

	D3D12_CLEAR_VALUE clearValue = {};
	clearValue.Format = format;
	RGD3D.GetDevice()->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
		D3D12_HEAP_FLAG_NONE,
		&desc,
		D3D12_RESOURCE_STATE_RENDER_TARGET,
		&clearValue,
		IID_PPV_ARGS(&m_texture2D)
	);

	// SRVの作成
	m_srvDesc.Texture2D.MipLevels = 1;
	m_srvDesc.Format = format;
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
	m_srvDesc = {};
	RGHEAPMGR.Free(m_srv.index);
}

void RgTexture2D::CreateCommandSetSRV(const ComPtr<ID3D12GraphicsCommandList>& command, const UINT paraNum)const
{
	command->SetGraphicsRootDescriptorTable(paraNum, m_srv.handle);
}



bool RgRenderTargets::CreateRT(
	const int w,
	const int h,
	std::vector<DXGI_FORMAT> format)
{
	if (format.size() == 0) {
		Release();
		return false;
	}
	// マルチレンダーターゲットの枚数
	m_multiRTCount = format.size();
	// RTV用ディスクリプタヒープ作成
	CreateDescriptorHeap();

	//==========================
	// テクスチャ作成
	//==========================
	for (int i = 0; i < m_multiRTCount; i++)
	{
		auto rt = std::make_shared<RgTexture2D>();
		rt->CreateTexAndSRV(w, h, format[i]);
		m_rt.push_back(rt);
	}
	//==========================
	// テクスチャにRTVを作成する
	//==========================
	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(
		m_heapRtv->GetCPUDescriptorHandleForHeapStart());
	for (int i = 0; i < m_multiRTCount; i++)
	{
		m_rtvHandles.push_back(rtvHandle);
		RGD3D.GetDevice()->CreateRenderTargetView(m_rt[i]->GetResource().Get(), nullptr, m_rtvHandles.back());
		rtvHandle.Offset(1, m_rtvDescriptorSize);
	}

	return true;
}

// ディスクリプタヒープの作成
bool RgRenderTargets::CreateDescriptorHeap()
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
	m_rtvDescriptorSize = RGD3D.GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

	return true;
}

void RgRenderTargets::SetRenderTargets(ComPtr<ID3D12GraphicsCommandList>& comList)
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
		m_multiRTCount, &m_rtvHandles[0], false, nullptr);
}

void RgRenderTargets::CreateCommandSetSRV(
	const UINT rtNum, 
	const ComPtr<ID3D12GraphicsCommandList>& command, 
	const UINT paraNum)const
{
	if (rtNum >= m_multiRTCount)return;
	auto barrier = CD3DX12_RESOURCE_BARRIER::Transition(
		m_rt[rtNum]->GetResource().Get(),
		D3D12_RESOURCE_STATE_RENDER_TARGET,
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE
	);
	command->ResourceBarrier(1, &barrier);

	m_rt[rtNum]->CreateCommandSetSRV(command, paraNum);
}