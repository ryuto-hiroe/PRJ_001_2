
#include "RgLibrary.h"

#include "RgResourceFactory.h"


std::shared_ptr<RgTexture2D> RgResourceFactory::GetTexture(const std::string& filename)
{
	//=======================================
	// Šù‚É“Ç‚İ‚ñ‚Å‚¢‚é‚©H
	//=======================================
	{
		auto it = m_TexManageMap.find(filename);
		if (it != m_TexManageMap.end())	// Šù‚É“Ç‚İ‚ñ‚Å‚¢‚½
		{
			return (*it).second;
		}
	}
	//=======================================
	// –¢ƒ[ƒh‚Ìê‡‚ÍV‚½‚Éì¬
	//=======================================
	std::shared_ptr<RgTexture2D> result = std::make_shared<RgTexture2D>();
	// “Ç‚İ‚İ
	if (result->LoadTexture(filename) == false) {
		// “Ç‚İ‚İ¸”s
		return nullptr;
	}
	// “o˜^
	{
		m_TexManageMap[filename] = result;
	}
	return result;
}

std::shared_ptr<RgModel> RgResourceFactory::GetModel(const std::string& filename)
{
	//=======================================
	// Šù‚É“Ç‚İ‚ñ‚Å‚¢‚é‚©H
	//=======================================
	{
		auto it = m_ModelManageMap.find(filename);
		if (it != m_ModelManageMap.end())	// Šù‚É“Ç‚İ‚ñ‚Å‚¢‚½
		{
			return (*it).second;
		}
	}
	//=======================================
	// –¢ƒ[ƒh‚Ìê‡‚ÍV‚½‚Éì¬
	//=======================================
	std::shared_ptr<RgModel> result = std::make_shared<RgModel>();
	// “Ç‚İ‚İ
	if (result->LoadModel(filename) == false) {
		// “Ç‚İ‚İ¸”s
		return nullptr;
	}
	// “o˜^
	{
		m_ModelManageMap[filename] = result;
	}
	return result;
}