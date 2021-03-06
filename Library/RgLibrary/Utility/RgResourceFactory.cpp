
#include "RgLibrary.h"

#include "RgResourceFactory.h"


std::shared_ptr<RgTexture2D> RgResourceFactory::GetTexture(const std::string& filename)
{
	//=======================================
	// 既に読み込んでいるか？
	//=======================================
	{
		auto it = m_TexManageMap.find(filename);
		if (it != m_TexManageMap.end())	// 既に読み込んでいた
		{
			return (*it).second;
		}
	}
	//=======================================
	// 未ロードの場合は新たに作成
	//=======================================
	std::shared_ptr<RgTexture2D> result = std::make_shared<RgTexture2D>();
	// 読み込み
	if (result->LoadTexture(filename) == false) {
		// 読み込み失敗
		return nullptr;
	}
	// 登録
	{
		m_TexManageMap[filename] = result;
	}
	return result;
}

std::shared_ptr<RgModel> RgResourceFactory::GetModel(const std::string& filename)
{
	//=======================================
	// 既に読み込んでいるか？
	//=======================================
	{
		auto it = m_ModelManageMap.find(filename);
		if (it != m_ModelManageMap.end())	// 既に読み込んでいた
		{
			return (*it).second;
		}
	}
	//=======================================
	// 未ロードの場合は新たに作成
	//=======================================
	std::shared_ptr<RgModel> result = std::make_shared<RgModel>();
	// 読み込み
	if (result->LoadModel(filename) == false) {
		// 読み込み失敗
		return nullptr;
	}
	// 登録
	{
		m_ModelManageMap[filename] = result;
	}
	return result;
}