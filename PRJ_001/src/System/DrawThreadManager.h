#pragma once


class DrawThreadManager
{
public:

	// �}���`�X���b�h�ŕ`��R�}���h���쐬����
	void CreateDrawCommands();

	// �e�R�}���h���X�g��ݒ肷��
	void CommandSetUp();

	// �I�u�W�F�N�g���X�g��Nullcheck������
	void CheakObjectList();

	// �R�}���h���X�g�����s
	void ExecuteCommandLists();

	//=======================
	//
	// �ݒ�֌W
	//
	//=======================
	// �X���b�h����ݒ�
	void InitThread();

	// �I�u�W�F�N�g��ǉ�
	void SetObject(const std::shared_ptr<Object>& p);

private:

	// thread num
	UINT m_threadNum = 1;

	// 
	std::vector<std::vector<std::weak_ptr<Object>>> m_objectList;
	// �`��R�}���h�쐬�p
	//	�X���b�h�����p������
	std::vector<RgCommandList> m_commandList;
};