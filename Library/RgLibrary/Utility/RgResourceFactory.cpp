
#include "RgLibrary.h"

#include "RgResourceFactory.h"


std::shared_ptr<RgTexture2D> RgResourceFactory::GetTexture(const std::string& filename)
{
	//=======================================
	// ���ɓǂݍ���ł��邩�H
	//=======================================
	{
		auto it = m_TexManageMap.find(filename);
		if (it != m_TexManageMap.end())	// ���ɓǂݍ���ł���
		{
			return (*it).second;
		}
	}
	//=======================================
	// �����[�h�̏ꍇ�͐V���ɍ쐬
	//=======================================
	std::shared_ptr<RgTexture2D> result = std::make_shared<RgTexture2D>();
	// �ǂݍ���
	if (result->LoadTexture(filename) == false) {
		// �ǂݍ��ݎ��s
		return nullptr;
	}
	// �o�^
	{
		m_TexManageMap[filename] = result;
	}
	return result;
}

std::shared_ptr<RgModel> RgResourceFactory::GetModel(const std::string& filename)
{
	//=======================================
	// ���ɓǂݍ���ł��邩�H
	//=======================================
	{
		auto it = m_ModelManageMap.find(filename);
		if (it != m_ModelManageMap.end())	// ���ɓǂݍ���ł���
		{
			return (*it).second;
		}
	}
	//=======================================
	// �����[�h�̏ꍇ�͐V���ɍ쐬
	//=======================================
	std::shared_ptr<RgModel> result = std::make_shared<RgModel>();
	// �ǂݍ���
	if (result->LoadModel(filename) == false) {
		// �ǂݍ��ݎ��s
		return nullptr;
	}
	// �o�^
	{
		m_ModelManageMap[filename] = result;
	}
	return result;
}