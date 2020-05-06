

#include "RgLibrary.h"

#include "RgPrimitiveShader.h"

bool RgPrimitiveShader::Init()
{
	HRESULT hr;

	//=================================
	// �R���p�C���ς݃V�F�[�_�̓ǂݍ���
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
	// rootSignature�̍쐬
	//====================================
	CD3DX12_DESCRIPTOR_RANGE cbv1, cbv2;
	cbv1.Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 0);	// b0 �I�u�W�F�N�g
	cbv2.Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 10);	// b10 �J����

	CD3DX12_ROOT_PARAMETER rootParams[2];
	rootParams[0].InitAsDescriptorTable(1, &cbv1, D3D12_SHADER_VISIBILITY_ALL);
	rootParams[1].InitAsDescriptorTable(1, &cbv2, D3D12_SHADER_VISIBILITY_ALL);

	// ���[�g�V�O�l�`���̍\�z
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

	// RootSignature �̐���
	hr = RGD3D.GetDevice()->CreateRootSignature(
		0,
		signature->GetBufferPointer(), signature->GetBufferSize(),
		IID_PPV_ARGS(&m_rootSignature)
	);
	if (FAILED(hr)) {
		return false;
	}

	//====================================
	// Graphic Pipline�̍쐬
	//====================================
	// �C���v�b�g���C�A�E�g
	D3D12_INPUT_ELEMENT_DESC inputElementDesc[] = {
	  { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(RgVertex_Pos, pos), D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
	};
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
	// ���[�g�V�O�l�`���̃Z�b�g
	psoDesc.pRootSignature = m_rootSignature.Get();
	psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE;
	// �}���`�T���v���ݒ�
	psoDesc.SampleDesc = { 1,0 };
	psoDesc.SampleMask = UINT_MAX; // �����Y���ƊG���o�Ȃ����x�����o�Ȃ��̂Œ���.

	hr = RGD3D.GetDevice()->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&m_pipeline));
	if (FAILED(hr)) {
		return false;
	}

	if (m_vb.Create(2) == false) {
		Release();
		return false;
	}

	//====================================
	// �萔�o�b�t�@�̍쐬
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
	// �p�C�v���C���X�e�[�g�̃Z�b�g
	comList->SetPipelineState(m_pipeline.Get());
	// ���[�g�V�O�l�`���̃Z�b�g
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
	// �萔�o�b�t�@�X�V
	m_cb0_Object.GetWork().mWorld = *mat;
	m_cb0_Object.GetWork().color = color;
	m_cb0_Object.WriteData();
	m_cb0_Object.CreateCommandSetCBV(comList, 0);

	// ���_�쐬
	RgVertex_Pos vertex[] = {
		pos1, pos2
	};

	// ���_�o�b�t�@�̍쐬
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
	// �萔�o�b�t�@�X�V
	m_cb0_Object.GetWork().mWorld = *mat;
	m_cb0_Object.GetWork().color = color;
	m_cb0_Object.WriteData();
	m_cb0_Object.CreateCommandSetCBV(comList, 0);

	model.CreateCommandSetCBV(comList);

	//comList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINELIST);
	comList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINESTRIP);

	comList->DrawInstanced(2, 1, 0, 0);
}