#pragma once

class RgResourceFactory
{
public:


	//===============================
	//
	// Texture
	//
	//===============================
	std::shared_ptr<RgTexture2D> GetTexture(const std::string& filename);


	//===============================
	// 
	// ���f��
	//
	//===============================
	std::shared_ptr<RgModel> GetModel(const std::string& filename);


private:

	// RgTexture2D�Ǘ��}�b�v
	std::unordered_map<std::string, std::shared_ptr<RgTexture2D>> m_TexManageMap;

	// RgModel�Ǘ��}�b�v
	std::unordered_map<std::string, std::shared_ptr<RgModel>> m_ModelManageMap;


	//===============================
	// �V���O���g���p�^�[��
	//===============================
private:
	//
	RgResourceFactory(){}

public:

	//
	static RgResourceFactory& GetInctance() {
		static RgResourceFactory ins;
		return ins;
	}
};
#define RGRESFACT RgResourceFactory::GetInctance()