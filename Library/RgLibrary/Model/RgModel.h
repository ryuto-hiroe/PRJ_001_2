#pragma once


// ���_�\����
struct RgVertex
{
	RgVec3 pos;
	RgVec3 normal;
	RgVec2 uv;
};

// �萔�o�b�t�@�p�\����
struct cbMaterial
{
	RgVec4	m_color = { 1, 1, 1, 1 };
	float	m_roughness = 1.0f;
	float	m_metallic = 0.0f;
	float	tmp[2];
};

//====================================================
// �}�e���A���f�[�^
//		�E�����x�[�X�p�}�e���A��
//
//====================================================
class RgMaterial
{
public:

	// �e�N�X�`��
	RgTexture2D m_tex;
	//
	Microsoft::glTF::AlphaMode alphaMode;

	// ������
	void Init();

	// CBV�ݒ�R�}���h�쐬
	void CreateCommandSetCBV(ComPtr<ID3D12GraphicsCommandList>& command, UINT paraNum)const
	{
		m_cb_Material.CreateCommandSetCBV(command, paraNum);
	}

	// �萔�o�b�t�@���擾
	RgStaticConstantBuffer<cbMaterial>& GetCB() { return m_cb_Material; }

private:

	// �萔�o�b�t�@
	RgStaticConstantBuffer<cbMaterial> m_cb_Material;
};


//====================================================
//
// ���b�V���v���~�e�B�u�N���X
//	�E���_�o�b�t�@�A�C���f�b�N�X�o�b�t�@�������Ă���
//
//====================================================
class RgMeshPrimitive
{
public:
	// ���_�E�C���f�b�N�X�o�b�t�@��ݒ肷��
	void SetToShader(const ComPtr<ID3D12GraphicsCommandList>& command)
	{
		command->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		command->IASetVertexBuffers(0, 1, &m_vertexView);
		command->IASetIndexBuffer(&m_indexView);
	}

	//==============================================
	//
	// �쐬�֌W
	//
	//==============================================

	// 
	bool CreateMeshPrimitive(const Microsoft::glTF::Document& doc,
		const Microsoft::glTF::MeshPrimitive& meshPrim, 
		std::shared_ptr<Microsoft::glTF::GLTFResourceReader> reader);


	//==============================================
	//
	// �擾�֌W
	//
	//==============================================
	// �}�e���A��ID�̎擾
	const int GetMaterialIndex()const { return m_materialIndex; }

	// �C���f�b�N�X�̐����擾
	const uint32_t GetIndexCount()const { return m_indexCount; }

private:
	// ���_�o�b�t�@
	ComPtr<ID3D12Resource1> m_vertexBuffer = nullptr;
	D3D12_VERTEX_BUFFER_VIEW m_vertexView;
	uint32_t m_vertexCount = 0;
	// �C���f�b�N�X�o�b�t�@
	ComPtr<ID3D12Resource1> m_indexBuffer = nullptr;
	D3D12_INDEX_BUFFER_VIEW m_indexView;
	uint32_t m_indexCount = 0;

	// �}�e���A���C���f�b�N�X
	int m_materialIndex = 0;
};


//====================================================
//
// ���b�V���f�[�^�̃N���X
//
//====================================================
class RgMesh
{
public:

	//==============================================
	//
	// �쐬�֌W
	//
	//==============================================

	// ���b�V���̍쐬
	bool CreateMesh(
		const Microsoft::glTF::Document& doc,
		const Microsoft::glTF::Mesh& mesh,
		std::shared_ptr<Microsoft::glTF::GLTFResourceReader> reader);


	//==============================================
	//
	// �擾�֌W
	//
	//==============================================
	// ���b�V���v���~�e�B�u�̎擾
	const std::vector<RgMeshPrimitive>& GetMeshPrimitives()const { 
		return m_meshPrimitives; 
	}

private:
	// ���b�V���v���~�e�B�u�z��
	std::vector<RgMeshPrimitive> m_meshPrimitives;

	// ���b�V��(����)�̃C���f�b�N�X
	int m_meshIndex = 0;
};

//====================================================
//
// ���f���p�N���X
//
//		�ERgModel�N���X��RgMesh��RgMaterial�N���X��z��Ŏ����Ă���
//====================================================
class RgModel
{
public:

	//==============================================
	//
	// �쐬�֌W
	//
	//==============================================
	// ���f���ǂݍ���
	bool LoadModel(const std::string& filename);

	// �W�I���g���f�[�^���쐬
	bool CreateModelGeometry(
		const Microsoft::glTF::Document& doc,
		std::shared_ptr<Microsoft::glTF::GLTFResourceReader> reader);

	// �}�e���A���f�[�^���쐬
	bool CreateModelMaterial(
		const Microsoft::glTF::Document& doc,
		std::shared_ptr<Microsoft::glTF::GLTFResourceReader> reader);

	// �{�[���f�[�^���쐬
	bool CreateModelBone(
		const Microsoft::glTF::Document& doc,
		std::shared_ptr<Microsoft::glTF::GLTFResourceReader> reader);

	//==============================================
	//
	// �擾�֌W
	//
	//==============================================
	// ���b�V���f�[�^�̎擾
	const std::vector<RgMesh>& GetMesh()const { return m_meshes; }

	// �}�e���A���f�[�^�̎擾
	const std::vector<RgMaterial>& GetMaterial()const { return m_materials; }


private:
	// ���f���t�@�C����
	std::string m_fileName = "";

	// ���b�V���f�[�^
	std::vector<RgMesh> m_meshes;
	// �}�e���A���f�[�^
	std::vector<RgMaterial> m_materials;

};



//==================================================
//
// ���p���f��
//
//==================================================
class RgLineModel
{
public:

	//===============================
	// �쐬
	//===============================
	void Create(const RgVec3& pos1, const RgVec3& pos2);

	// ���_�X�V
	void UpdateVerTex(const RgVec3& pos1, const RgVec3& pos2);

	//===============================
	// �ݒ�֌W
	//===============================
	void CreateCommandSetCBV(ComPtr<ID3D12GraphicsCommandList>& command)const;

private:
	struct LineVertex
	{
		RgVec3 pos;
	};
	LineVertex m_vertex[2];
	//RgVec3 m_pos1, m_pos2;

	// ���_�o�b�t�@
	ComPtr<ID3D12Resource1> m_buffer;
	// ���_�o�b�t�@�r���[
	D3D12_VERTEX_BUFFER_VIEW  m_bufferView;
};