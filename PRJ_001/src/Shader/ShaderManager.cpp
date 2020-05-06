
#include "main.h"

#include "ShaderManager.h"

bool ShaderManager::Init()
{
	//===============================
	// シェーダクラスの初期化
	//===============================
	if (m_PrimSh.Init() == false) {
		Release();
		return false;
	}
	if (m_ModelSh.Init() == false) {
		Release();
		return false;
	}
	//===============================
	// 定数バッファの初期化
	//===============================
	if (m_cb10_Camera.Create() == false) {
		Release();
		return false;
	}

	return true;
}

void ShaderManager::Release()
{
	
}