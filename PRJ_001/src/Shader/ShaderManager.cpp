
#include "main.h"

#include "ShaderManager.h"

bool ShaderManager::Init()
{
	//===============================
	// �V�F�[�_�N���X�̏�����
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
	// �萔�o�b�t�@�̏�����
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