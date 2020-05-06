#pragma once


// 頂点構造体
struct RgVertex
{
	RgVec3 pos;
	RgVec3 normal;
	RgVec2 uv;
};

// 定数バッファ用構造体
struct cbMaterial
{
	RgVec4	m_color = { 1, 1, 1, 1 };
	float	m_roughness = 1.0f;
	float	m_metallic = 0.0f;
	float	tmp[2];
};

//====================================================
// マテリアルデータ
//		・物理ベース用マテリアル
//
//====================================================
class RgMaterial
{
public:

	// テクスチャ
	RgTexture2D m_tex;
	//
	Microsoft::glTF::AlphaMode alphaMode;

	// 初期化
	void Init();

	// CBV設定コマンド作成
	void CreateCommandSetCBV(ComPtr<ID3D12GraphicsCommandList>& command, UINT paraNum)const
	{
		m_cb_Material.CreateCommandSetCBV(command, paraNum);
	}

	// 定数バッファを取得
	RgStaticConstantBuffer<cbMaterial>& GetCB() { return m_cb_Material; }

private:

	// 定数バッファ
	RgStaticConstantBuffer<cbMaterial> m_cb_Material;
};


//====================================================
//
// メッシュプリミティブクラス
//	・頂点バッファ、インデックスバッファを持っている
//
//====================================================
class RgMeshPrimitive
{
public:
	// 頂点・インデックスバッファを設定する
	void SetToShader(const ComPtr<ID3D12GraphicsCommandList>& command)
	{
		command->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		command->IASetVertexBuffers(0, 1, &m_vertexView);
		command->IASetIndexBuffer(&m_indexView);
	}

	//==============================================
	//
	// 作成関係
	//
	//==============================================

	// 
	bool CreateMeshPrimitive(const Microsoft::glTF::Document& doc,
		const Microsoft::glTF::MeshPrimitive& meshPrim, 
		std::shared_ptr<Microsoft::glTF::GLTFResourceReader> reader);


	//==============================================
	//
	// 取得関係
	//
	//==============================================
	// マテリアルIDの取得
	const int GetMaterialIndex()const { return m_materialIndex; }

	// インデックスの数を取得
	const uint32_t GetIndexCount()const { return m_indexCount; }

private:
	// 頂点バッファ
	ComPtr<ID3D12Resource1> m_vertexBuffer = nullptr;
	D3D12_VERTEX_BUFFER_VIEW m_vertexView;
	uint32_t m_vertexCount = 0;
	// インデックスバッファ
	ComPtr<ID3D12Resource1> m_indexBuffer = nullptr;
	D3D12_INDEX_BUFFER_VIEW m_indexView;
	uint32_t m_indexCount = 0;

	// マテリアルインデックス
	int m_materialIndex = 0;
};


//====================================================
//
// メッシュデータのクラス
//
//====================================================
class RgMesh
{
public:

	//==============================================
	//
	// 作成関係
	//
	//==============================================

	// メッシュの作成
	bool CreateMesh(
		const Microsoft::glTF::Document& doc,
		const Microsoft::glTF::Mesh& mesh,
		std::shared_ptr<Microsoft::glTF::GLTFResourceReader> reader);


	//==============================================
	//
	// 取得関係
	//
	//==============================================
	// メッシュプリミティブの取得
	const std::vector<RgMeshPrimitive>& GetMeshPrimitives()const { 
		return m_meshPrimitives; 
	}

private:
	// メッシュプリミティブ配列
	std::vector<RgMeshPrimitive> m_meshPrimitives;

	// メッシュ(自分)のインデックス
	int m_meshIndex = 0;
};

//====================================================
//
// モデル用クラス
//
//		・RgModelクラスはRgMeshとRgMaterialクラスを配列で持っている
//====================================================
class RgModel
{
public:

	//==============================================
	//
	// 作成関係
	//
	//==============================================
	// モデル読み込み
	bool LoadModel(const std::string& filename);

	// ジオメトリデータを作成
	bool CreateModelGeometry(
		const Microsoft::glTF::Document& doc,
		std::shared_ptr<Microsoft::glTF::GLTFResourceReader> reader);

	// マテリアルデータを作成
	bool CreateModelMaterial(
		const Microsoft::glTF::Document& doc,
		std::shared_ptr<Microsoft::glTF::GLTFResourceReader> reader);

	// ボーンデータを作成
	bool CreateModelBone(
		const Microsoft::glTF::Document& doc,
		std::shared_ptr<Microsoft::glTF::GLTFResourceReader> reader);

	//==============================================
	//
	// 取得関係
	//
	//==============================================
	// メッシュデータの取得
	const std::vector<RgMesh>& GetMesh()const { return m_meshes; }

	// マテリアルデータの取得
	const std::vector<RgMaterial>& GetMaterial()const { return m_materials; }


private:
	// モデルファイル名
	std::string m_fileName = "";

	// メッシュデータ
	std::vector<RgMesh> m_meshes;
	// マテリアルデータ
	std::vector<RgMaterial> m_materials;

};



//==================================================
//
// 線用モデル
//
//==================================================
class RgLineModel
{
public:

	//===============================
	// 作成
	//===============================
	void Create(const RgVec3& pos1, const RgVec3& pos2);

	// 頂点更新
	void UpdateVerTex(const RgVec3& pos1, const RgVec3& pos2);

	//===============================
	// 設定関係
	//===============================
	void CreateCommandSetCBV(ComPtr<ID3D12GraphicsCommandList>& command)const;

private:
	struct LineVertex
	{
		RgVec3 pos;
	};
	LineVertex m_vertex[2];
	//RgVec3 m_pos1, m_pos2;

	// 頂点バッファ
	ComPtr<ID3D12Resource1> m_buffer;
	// 頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW  m_bufferView;
};