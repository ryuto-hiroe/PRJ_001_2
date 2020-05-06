#pragma once


class RgPrimitive
{
public:

	// ������
	bool Init();
	// ���
	void Release();

	// �`��O����
	void SetUp();


	// ���_�\����
	//struct RgVertex
	//{
	//	RgVec3 pos;		// ���_���W
	//	RgVec2 uv;		// UV���W
	//};

private:
	// �V�F�[�_�I�u�W�F�N�g
	ComPtr<ID3DBlob>  m_vs, m_ps;
	// 
	ComPtr<ID3D12RootSignature> m_rootSignature;
	ComPtr<ID3D12PipelineState> m_pipeline;

	//=======================
	// ���\�[�X
	//=======================
	// ���_�E�C���f�b�N�X�o�b�t�@
	ComPtr<ID3D12Resource1> m_vertexBuffer;
	ComPtr<ID3D12Resource1> m_indexBuffer;
	D3D12_VERTEX_BUFFER_VIEW  m_vertexBufferView;
	D3D12_INDEX_BUFFER_VIEW   m_indexBufferView;
	UINT  m_indexCount;
};