

#include "RgLibrary.h"

#include "RgPrimitiveShader.h"

bool RgPrimitiveShader::Init()
{
	HRESULT hr;

	//=================================
	// コンパイル済みシェーダの読み込み
	//=================================
	hr = D3DReadFileToBlob(L"x64/Debug/RgPrimitive_VS.cso", &m_vs);
	if (FAILED(hr)) {
		return false;
	}
	hr = D3DReadFileToBlob(L"x64/Debug/RgPrimitive_PS.cso", &m_ps);
	if (FAILED(hr)) {
		return false;
	}

	//====================================
	// rootSignatureの作成
	//====================================
	CD3DX12_DESCRIPTOR_RANGE cbv1, cbv2;
	cbv1.Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 0);	// b0 オブジェクト
	cbv2.Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 10);	// b10 カメラ

	CD3DX12_ROOT_PARAMETER rootParams[2];
	rootParams[0].InitAsDescriptorTable(1, &cbv1, D3D12_SHADER_VISIBILITY_ALL);
	rootParams[1].InitAsDescriptorTable(1, &cbv2, D3D12_SHADER_VISIBILITY_ALL);

	// ルートシグネチャの構築
	CD3DX12_ROOT_SIGNATURE_DESC rootSigDesc{};
	rootSigDesc.Init(
		_countof(rootParams), rootParams,		//pParameters
		1, &CD3DX12_STATIC_SAMPLER_DESC(0),		//pStaticSamplers
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT
	);
	ComPtr<ID3DBlob> signature;
	ComPtr<ID3DBlob> errBlob;
	hr = D3D12SerializeRootSignature(&rootSigDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &signature, &errBlob);
	if (FAILED(hr)) {
		return false;
	}

	// RootSignature の生成
	hr = RGD3D.GetDevice()->CreateRootSignature(
		0,
		signature->GetBufferPointer(), signature->GetBufferSize(),
		IID_PPV_ARGS(&m_rootSignature)
	);
	if (FAILED(hr)) {
		return false;
	}

	//====================================
	// Graphic Piplineの作成
	//====================================
	// インプットレイアウト
	D3D12_INPUT_ELEMENT_DESC inputElementDesc[] = {
	  { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(RgVertex_Pos, pos), D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
	};
	D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc{};

	// シェーダーのセット
	psoDesc.VS = CD3DX12_SHADER_BYTECODE(m_vs.Get());
	psoDesc.PS = CD3DX12_SHADER_BYTECODE(m_ps.Get());
	//
	psoDesc.InputLayout = { inputElementDesc, _countof(inputElementDesc) };
	// ブレンドステート設定
	psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
	// ラスタライザーステート
	psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	// 出力先は1ターゲット
	psoDesc.NumRenderTargets = 1;
	psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	// デプスバッファのフォーマットを設定
	psoDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;
	psoDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	// ルートシグネチャのセット
	psoDesc.pRootSignature = m_rootSignature.Get();
	psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE;
	// マルチサンプル設定
	psoDesc.SampleDesc = { 1,0 };
	psoDesc.SampleMask = UINT_MAX; // これを忘れると絵が出ない＆警告も出ないので注意.

	hr = RGD3D.GetDevice()->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&m_pipeline));
	if (FAILED(hr)) {
		return false;
	}

	if (m_vb.Create(2) == false) {
		Release();
		return false;
	}

	//====================================
	// 定数バッファの作成
	//====================================
	if (m_cb0_Object.Create() == false) {
		Release();
		return false;
	}
	if (m_cb10_Camera.Create() == false) {
		Release();
		return false;
	}

	return true;
}

void RgPrimitiveShader::Release()
{
	
}

void RgPrimitiveShader::SetUp(ComPtr<ID3D12GraphicsCommandList>& comList)
{
	// パイプラインステートのセット
	comList->SetPipelineState(m_pipeline.Get());
	// ルートシグネチャのセット
	comList->SetGraphicsRootSignature(m_rootSignature.Get());
}

void RgPrimitiveShader::DrawLine(
	const RgVec3& pos1,
	const RgVec3& pos2,
	const RgVec4& color,
	ComPtr<ID3D12GraphicsCommandList>& comList,
	const RgMatrix* mat
)
{
	// 定数バッファ更新
	m_cb0_Object.GetWork().mWorld = *mat;
	m_cb0_Object.GetWork().color = color;
	m_cb0_Object.WriteData();
	m_cb0_Object.CreateCommandSetCBV(comList, 0);

	// 頂点作成
	RgVertex_Pos vertex[] = {
		pos1, pos2
	};

	// 頂点バッファの作成
	m_vb.GetWork()[0].pos = pos1;
	m_vb.GetWork()[1].pos = pos2;
	m_vb.WriteData();
	m_vb.CreateCommandSetCBV(comList);
	comList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINELIST);

	comList->DrawInstanced(2, 1, 0, 0);
}

void RgPrimitiveShader::DrawLine(
	const RgLineModel& model, 
	const RgVec4& color, 
	ComPtr<ID3D12GraphicsCommandList>& comList,
	const RgMatrix* mat)
{
	// 定数バッファ更新
	m_cb0_Object.GetWork().mWorld = *mat;
	m_cb0_Object.GetWork().color = color;
	m_cb0_Object.WriteData();
	m_cb0_Object.CreateCommandSetCBV(comList, 0);

	model.CreateCommandSetCBV(comList);

	//comList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINELIST);
	comList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINESTRIP);

	comList->DrawInstanced(2, 1, 0, 0);
}