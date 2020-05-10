
#include "main.h"

#include "ModelShader.h"

bool ModelShader::Init()
{
	HRESULT hr;

	//============================
	// コンパイル済みシェーダの読み込み
	//============================
	hr = D3DReadFileToBlob(L"x64/Debug/ModelShader_VS.cso", &m_vs);
	if (FAILED(hr)) {
		return false;
	}
	hr = D3DReadFileToBlob(L"x64/Debug/ModelShader_PS.cso", &m_ps);
	if (FAILED(hr)) {
		return false;
	}

	//====================================
	// rootSignatureの作成
	//====================================
	CD3DX12_DESCRIPTOR_RANGE cbv, cbv2, cbv3,srv;
	
	cbv.Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 0);	// b0 オブジェクト
	cbv2.Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 1);	// b1 マテリアル
	cbv3.Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 10);	// b10 カメラ
	srv.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);	// t0 マテリアルテクスチャ
	CD3DX12_ROOT_PARAMETER rootParams[4];
	rootParams[0].InitAsDescriptorTable(1, &cbv, D3D12_SHADER_VISIBILITY_ALL);
	rootParams[1].InitAsDescriptorTable(1, &cbv2, D3D12_SHADER_VISIBILITY_ALL);
	rootParams[2].InitAsDescriptorTable(1, &cbv3, D3D12_SHADER_VISIBILITY_ALL);
	rootParams[3].InitAsDescriptorTable(1, &srv, D3D12_SHADER_VISIBILITY_PIXEL);

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
	  { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(RgVertex, pos), D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
	  { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(RgVertex, normal), D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
	  { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,0, offsetof(RgVertex, uv), D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
	};

	// パイプラインステートオブジェクトの生成.
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
	//psoDesc.DepthStencilState.DepthEnable = FALSE;
	//psoDesc.DepthStencilState.StencilEnable = FALSE;
	// ルートシグネチャのセット
	psoDesc.pRootSignature = m_rootSignature.Get();
	psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	// マルチサンプル設定
	psoDesc.SampleDesc = { 1,0 };
	psoDesc.SampleMask = UINT_MAX; // これを忘れると絵が出ない＆警告も出ないので注意.

	hr = RGD3D.GetDevice()->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&m_pipeline));
	if (FAILED(hr)) {
		return false;
	}


	//====================================
	// 定数バッファの作成
	//====================================
	if (m_cb0_Object.Create() == false) {
		Release();
		return false;
	}
	if (m_cb1_Material.Create() == false) {
		Release();
		return false;
	}

	return true;
}


void ModelShader::Release()
{

}

void ModelShader::SetUp()
{
	// パイプラインステートのセット
	RGD3D.GetCL()->SetPipelineState(m_pipeline.Get());
	// ルートシグネチャのセット
	RGD3D.GetCL()->SetGraphicsRootSignature(m_rootSignature.Get());
}

void ModelShader::DrawModel(const RgModel& model)
{
	//
	m_cb0_Object.WriteData();
	m_cb0_Object.CreateCommandSetCBV(0);

	for (auto&& mesh : model.GetMesh())
	{
		for (auto primitive : mesh.GetMeshPrimitives())
		{
			auto& material = model.GetMaterial()[primitive.GetMaterialIndex()];

			// マテリアルCBV設定
			material.CreateCommandSetCBV(1);

			// メッシュ情報をセット
			primitive.SetToShader();

			// マテリアルテクスチャ
			material.m_tex.CreateCommandSetSRV(3);

			// 描画
			RGD3D.GetCL()->DrawIndexedInstanced(primitive.GetIndexCount(), 1, 0, 0, 0);
		}
	}
}