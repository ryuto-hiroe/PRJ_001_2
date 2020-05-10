#pragma once

struct RgVertex_Pos
{
	RgVec3 pos;
};

class RgPrimitiveShader
{
public:

	//
	bool Init();

	//
	void Release();

	// �`��O����
	void SetUp();

	// �J�����ݒ�
	void SetCamera(const RgMatrix& mV, const RgMatrix& mP)
	{
		m_cb10_Camera.GetWork().mView = mV;
		m_cb10_Camera.GetWork().mProj = mP;
		m_cb10_Camera.WriteData();
		//m_cb10_Camera.CreateCommandSetCBV(RGD3D.m_commandList, 1);
	}

	//===========================
	// �`��֐�
	//===========================
	// ���̕`��
	void DrawLine(
		const RgVec3& pos1,
		const RgVec3& pos2,
		const RgVec4& color,
		const RgMatrix* mat = (RgMatrix*)&RgMatrix::Identity
	);

	void DrawLine(
		const RgLineModel& model, 
		const RgVec4& color,
		const RgMatrix* mat = (RgMatrix*)&RgMatrix::Identity);


private:

	// �V�F�[�_�I�u�W�F�N�g
	ComPtr<ID3DBlob>  m_vs, m_ps;
	// 
	ComPtr<ID3D12RootSignature> m_rootSignature;
	ComPtr<ID3D12PipelineState> m_pipeline;

	// ���_�o�b�t�@
	RgDynamicVertexBuffer<RgVertex_Pos> m_vb;


	//=========================
	// �萔�o�b�t�@
	//=========================
	struct cbObject
	{
		RgMatrix mWorld;
		RgVec4 color;
	};
	RgDynamicConstantBuffer<cbObject> m_cb0_Object;

	struct cbCamera
	{
		RgMatrix mView;
		RgMatrix mProj;
	};
	RgStaticConstantBuffer<cbCamera> m_cb10_Camera;
};