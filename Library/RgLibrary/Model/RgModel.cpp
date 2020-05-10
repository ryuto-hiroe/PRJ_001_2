

#include "RgLibrary.h"
#include "RgModel.h"


bool RgModel::LoadModel(const std::string& filename)
{
	// モデルファイル名を記憶
	m_fileName = filename;

	auto modelFilePath = std::filesystem::path(filename);

	if (modelFilePath.is_relative())
	{
		auto current = std::filesystem::current_path();
		current /= modelFilePath;
		current.swap(modelFilePath);
	}
	auto reader = std::make_unique<StreamReader>(modelFilePath.parent_path());
	auto glbStream = reader->GetInputStream(modelFilePath.filename().u8string());
	auto glbResourceReader = std::make_shared<Microsoft::glTF::GLBResourceReader>(std::move(reader), std::move(glbStream));
	auto document = Microsoft::glTF::Deserialize(glbResourceReader->GetJson());

	if (CreateModelGeometry(document, glbResourceReader) == false)return false;
	if (CreateModelMaterial(document, glbResourceReader) == false)return false;
	if (CreateModelBone(document, glbResourceReader) == false)return false;

	return true;
}

bool RgModel::CreateModelGeometry(
	const Microsoft::glTF::Document& doc,
	std::shared_ptr<Microsoft::glTF::GLTFResourceReader> reader)
{
	using namespace Microsoft::glTF;

	for (const auto& mesh : doc.meshes.Elements())
	{
		RgMesh rgMesh;

		if (rgMesh.CreateMesh(doc, mesh, reader) == false) {
			return false;
		}

		m_meshes.push_back(rgMesh);
	}

	return true;
}

bool RgModel::CreateModelMaterial(
	const Microsoft::glTF::Document& doc,
	std::shared_ptr<Microsoft::glTF::GLTFResourceReader> reader)
{
	int textureIndex = 0;
	for (auto& m : doc.materials.Elements())
	{
		auto textureId = m.metallicRoughness.baseColorTexture.textureId;
		if (textureId.empty())
		{
			textureId = m.normalTexture.textureId;
		}
		
		auto& color = m.metallicRoughness.baseColorFactor;
		auto& roughness = m.metallicRoughness.roughnessFactor;
		auto& metaric = m.metallicRoughness.metallicFactor;
		auto& texture = doc.textures.Get(textureId);
		auto& image = doc.images.Get(texture.imageId);
		auto imageBufferView = doc.bufferViews.Get(image.bufferViewId);
		auto imageData = reader->ReadBinaryData<char>(doc, imageBufferView);

		// マテリアル作成
		RgMaterial material;
		material.Init();
		// 定数バッファ
		material.GetCB().GetWork().m_color = { color.r, color.g, color.b, color.a };
		material.GetCB().GetWork().m_roughness = roughness;
		material.GetCB().GetWork().m_metallic = metaric;
		material.GetCB().WriteData_AllBuffer();
		//
		material.alphaMode		= m.alphaMode;
		material.m_tex.LoadTextureFromMemory(imageData);

		m_materials.push_back(material);
	}

	return true;
}

bool RgModel::CreateModelBone(
	const Microsoft::glTF::Document& doc,
	std::shared_ptr<Microsoft::glTF::GLTFResourceReader> reader)
{
	using namespace Microsoft::glTF;

	for (const auto node : doc.nodes.Elements())
	{
		auto transType = node.GetTransformationType();
		auto testTRS = node.HasIdentityTRS();
		auto testVT = node.HasValidTransformType();
		node.skinId;

	}

	return true;
}



bool RgMesh::CreateMesh(
	const Microsoft::glTF::Document& doc,
	const Microsoft::glTF::Mesh& mesh,
	std::shared_ptr<Microsoft::glTF::GLTFResourceReader> reader)
{
	using namespace Microsoft::glTF;

	for (const auto& meshPrimitive : mesh.primitives)
	{
		RgMeshPrimitive meshPrim;

		if (meshPrim.CreateMeshPrimitive(doc, meshPrimitive, reader) == false)
		{
			return false;
		}

		m_meshPrimitives.push_back(meshPrim);
	}

	return true;
}





bool RgMeshPrimitive::CreateMeshPrimitive(
	const Microsoft::glTF::Document& doc,
	const Microsoft::glTF::MeshPrimitive& meshPrim,
	std::shared_ptr<Microsoft::glTF::GLTFResourceReader> reader)
{
	using namespace Microsoft::glTF;

	std::vector<RgVertex> vertices;
	std::vector<uint32_t> indices;

	// 頂点位置情報アクセッサの取得
	auto& idPos = meshPrim.GetAttributeAccessorId(ACCESSOR_POSITION);
	auto& accPos = doc.accessors.Get(idPos);
	// 法線情報アクセッサの取得
	auto& idNrm = meshPrim.GetAttributeAccessorId(ACCESSOR_NORMAL);
	auto& accNrm = doc.accessors.Get(idNrm);
	// テクスチャ座標情報アクセッサの取得
	auto& idUV = meshPrim.GetAttributeAccessorId(ACCESSOR_TEXCOORD_0);
	auto& accUV = doc.accessors.Get(idUV);
	// 頂点インデックス用アクセッサの取得
	auto& idIndex = meshPrim.indicesAccessorId;
	auto& accIndex = doc.accessors.Get(idIndex);

	// アクセッサからデータ列を取得
	auto vertPos = reader->ReadBinaryData<float>(doc, accPos);
	auto vertNrm = reader->ReadBinaryData<float>(doc, accNrm);
	auto vertUV = reader->ReadBinaryData<float>(doc, accUV);

	// 頂点バッファ
	auto vertexCount = accPos.count;
	for (uint32_t i = 0; i < vertexCount; ++i)
	{
		// 頂点データの構築
		int vid0 = 3 * i, vid1 = 3 * i + 1, vid2 = 3 * i + 2;
		int tid0 = 2 * i, tid1 = 2 * i + 1;
		vertices.emplace_back(
			RgVertex{
			  RgVec3(vertPos[vid0], vertPos[vid1],vertPos[vid2]),
			  RgVec3(vertNrm[vid0], vertNrm[vid1],vertNrm[vid2]),
			  RgVec2(vertUV[tid0],vertUV[tid1])
			}
		);
	}

	// インデックスバッファ
	if (accIndex.componentType == COMPONENT_UNSIGNED_SHORT) {
		auto indexData = reader->ReadBinaryData<uint16_t>(doc, accIndex);
		for (auto&& index : indexData) {
			indices.push_back(index);
		}
	}
	else {
		indices = reader->ReadBinaryData<uint32_t>(doc, accIndex);
	}

	auto vbSize = UINT(sizeof(RgVertex) * vertices.size());
	auto ibSize = UINT(sizeof(uint32_t) * indices.size());
	auto vb = Rg::CreateBuffer(vbSize, vertices.data());
	D3D12_VERTEX_BUFFER_VIEW vbView;
	vbView.BufferLocation = vb->GetGPUVirtualAddress();
	vbView.SizeInBytes = vbSize;
	vbView.StrideInBytes = sizeof(RgVertex);
	m_vertexBuffer = vb;
	m_vertexView = vbView;

	auto ib = Rg::CreateBuffer(ibSize, indices.data());
	D3D12_INDEX_BUFFER_VIEW ibView;
	ibView.BufferLocation = ib->GetGPUVirtualAddress();
	ibView.Format = DXGI_FORMAT_R32_UINT;
	ibView.SizeInBytes = ibSize;
	m_indexBuffer = ib;
	m_indexView = ibView;

	m_vertexCount = UINT(vertices.size());
	m_indexCount = UINT(indices.size());
	m_materialIndex = int(doc.materials.GetIndex(meshPrim.materialId));

	return true;
}



void RgMaterial::Init()
{
	// 定数バッファを作成
	m_cb_Material.Create();
}


//
void RgLineModel::Create(const RgVec3& pos1, const RgVec3& pos2)
{
	m_vertex[0].pos = pos1;
	m_vertex[1].pos = pos2;

	m_buffer = Rg::CreateBuffer(sizeof(m_vertex), m_vertex);

	m_bufferView.BufferLocation = m_buffer->GetGPUVirtualAddress();
	m_bufferView.SizeInBytes = sizeof(m_vertex);
	m_bufferView.StrideInBytes = sizeof(LineVertex);
}

void RgLineModel::CreateCommandSetCBV()const
{
	RGD3D.GetCL()->IASetVertexBuffers(0, 1, &m_bufferView);
}