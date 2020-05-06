
#include "RgLibrary.h"
#include "RgPrimitive.h"

bool RgPrimitive::Init()
{
	HRESULT hr;

	//====================================
	// �V�F�[�_�̃R���p�C��
	//====================================
	ComPtr<ID3DBlob> errBlob;
	//// ���_�V�F�[�_
	//hr = RGD3D.CompileShaderFromFile(L"src/Shader/Sample_VS.hlsl", L"vs_6_0", m_vs, errBlob);
	//if (FAILED(hr)) {
	//	return false;
	//}
	//// �s�N�Z���V�F�[�_
	//hr = RGD3D.CompileShaderFromFile(L"src/Shader/Sample_PS.hlsl", L"ps_6_0", m_ps, errBlob);
	//if (FAILED(hr)) {
	//	return false;
	//}

	UINT compileFlags = 0;
#ifdef _DEBUG
	compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

	hr = D3DCompileFromFile(L"src/Shader/Sample_VS.hlsl", nullptr, nullptr, "main", "vs_5_0", compileFlags, 0, &m_vs, nullptr);
	if (FAILED(hr)) {
		return false;
	}
	hr = D3DCompileFromFile(L"src/Shader/Sample_PS.hlsl", nullptr, nullptr, "main", "ps_5_0", compileFlags, 0, &m_ps, nullptr);
	if (FAILED(hr)) {
		return false;
	}

	//====================================
	// ���_�A�C���f�b�N�X�o�b�t�@�̍쐬
	//====================================
	//// ���_�f�[�^�쐬
	//RgVertex vertex[] = {
	//	{{-0.5f, 0.5f, 0.0f}, {0.0f, 0.0f}},
	//	{{0.5f, -0.5f, 0.0f}, {1.0f, 1.0f}},
	//	{ {-0.5f, -0.5f, 0.0f}, {0.0f, 1.0f}},
	//	{ {0.5f, 0.5f, 0.0f}, {1.0f, 0.0f}}
	//};
	//uint16_t index[] = { 
	//	//1, 3, 0 , 2, 1, 0 
	//	0, 3, 1 , 0, 1, 2 
	//};
	//
	//// ���_�A�C���f�b�N�X�o�b�t�@�̍쐬
	//m_vertexBuffer	= Rg::CreateBuffer(sizeof(vertex), vertex);
	//m_indexBuffer	= Rg::CreateBuffer(sizeof(index), index);
	//m_indexCount	= _countof(index);
	//// �e�o�b�t�@�̃r���[�𐶐�.
	//m_vertexBufferView.BufferLocation	= m_vertexBuffer->GetGPUVirtualAddress();
	//m_vertexBufferView.SizeInBytes		= sizeof(vertex);
	//m_vertexBufferView.StrideInBytes	= sizeof(RgVertex);
	//m_indexBufferView.BufferLocation	= m_indexBuffer->GetGPUVirtualAddress();
	//m_indexBufferView.SizeInBytes		= sizeof(index);
	//m_indexBufferView.Format			= DXGI_FORMAT_R16_UINT;
	
	//====================================
	// rootSignature�̍쐬
	//====================================
	CD3DX12_DESCRIPTOR_RANGE cbv, cbv2, srv;
	cbv.Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 0); // b0 ���W�X�^
	cbv2.Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 1); // b1 ���W�X�^
	srv.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0); // t0
	CD3DX12_ROOT_PARAMETER rootParams[3];
	rootParams[0].InitAsDescriptorTable(1, &cbv, D3D12_SHADER_VISIBILITY_ALL);
	rootParams[1].InitAsDescriptorTable(1, &cbv2, D3D12_SHADER_VISIBILITY_VERTEX);
	rootParams[2].InitAsDescriptorTable(1, &srv, D3D12_SHADER_VISIBILITY_PIXEL);

	// ���[�g�V�O�l�`���̍\�z
	CD3DX12_ROOT_SIGNATURE_DESC rootSigDesc{};
	rootSigDesc.Init(
		_countof(rootParams), rootParams,		//pParameters
		1, &CD3DX12_STATIC_SAMPLER_DESC(0),		//pStaticSamplers
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT
	);
	ComPtr<ID3DBlob> signature;
	hr = D3D12SerializeRootSignature(&rootSigDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &signature, &errBlob);
	if (FAILED(hr)) {
		return false;
	}

	// RootSignature �̐���
	hr = RGD3D.GetDevice()->CreateRootSignature(
		0,
		signature->GetBufferPointer(), signature->GetBufferSize(),
		IID_PPV_ARGS(&m_rootSignature)
	);
	if (FAILED(hr))	{
		return false;
	}

	//====================================
	// Graphic Pipline�̍쐬
	//====================================
	// �C���v�b�g���C�A�E�g
	D3D12_INPUT_ELEMENT_DESC inputElementDesc[] = {
	  { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(RgVertex, pos), D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
	  { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(RgVertex, normal), D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
	  { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,0, offsetof(RgVertex, uv), D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
	};

	// �p�C�v���C���X�e�[�g�I�u�W�F�N�g�̐���.
	D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc{};
	// �V�F�[�_�[�̃Z�b�g
	psoDesc.VS = CD3DX12_SHADER_BYTECODE(m_vs.Get());
	psoDesc.PS = CD3DX12_SHADER_BYTECODE(m_ps.Get());
	//
	psoDesc.InputLayout = { inputElementDesc, _countof(inputElementDesc) };
	// �u�����h�X�e�[�g�ݒ�
	psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
	// ���X�^���C�U�[�X�e�[�g
	psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	// �o�͐��1�^�[�Q�b�g
	psoDesc.NumRenderTargets = 1;
	psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	// �f�v�X�o�b�t�@�̃t�H�[�}�b�g��ݒ�
	psoDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;
	psoDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	//psoDesc.DepthStencilState.DepthEnable = FALSE;
	//psoDesc.DepthStencilState.StencilEnable = FALSE;
	// ���[�g�V�O�l�`���̃Z�b�g
	psoDesc.pRootSignature = m_rootSignature.Get();
	psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	// �}���`�T���v���ݒ�
	psoDesc.SampleDesc = { 1,0 };
	psoDesc.SampleMask = UINT_MAX; // �����Y���ƊG���o�Ȃ����x�����o�Ȃ��̂Œ���.

	hr = RGD3D.GetDevice()->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&m_pipeline));
	if (FAILED(hr)) {
		return false;
	}

	return true;
}

void RgPrimitive::Release()
{

}