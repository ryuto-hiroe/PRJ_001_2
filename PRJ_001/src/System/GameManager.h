#pragma once

class Object;

// �R���|�[�l���g�N���X
#include "System/ComponentSystem/BaseComponent.h"
#include "System/ComponentSystem/ModelComponent.h"


//==========================================
//
// �I�u�W�F�N�g�N���X
//
//==========================================
class Object : public std::enable_shared_from_this<Object>
{
public:

	//===============================
	//
	// �������֐�
	//
	//===============================
	// �I�u�W�F�N�g�������ɌĂ΂��
	void Awake();

	// 
	void Start();

	//===============================
	//
	// �X�V�֐�
	//
	//===============================
	//
	void Update();

	//===============================
	//
	// �`��֐�
	//
	//===============================
	//
	void Draw();

	//===============================
	//
	// Serialize / Deserialize
	//
	//===============================


	//===============================
	//
	// Editor�p
	//
	//===============================
	// ImGui�̏���
	void ImGuiUpdate();

	//===============================
	//
	// �e�q�\���֌W
	//
	//===============================
	// �q���X�g�擾
	const std::list<std::shared_ptr<Object>>& GetChildren() { return m_children; }

	// �e���擾
	std::shared_ptr<Object> GetParent() const { return m_parent.lock(); }

	// �w�肵���q���폜(�����ɏ������߁A�Ăяo�����͒���)
	void RemoveChild(const std::shared_ptr<Object>& obj);

	// �e��ݒ�
	void SetParent(const std::shared_ptr<Object>& parent);

	//===============================
	//
	// �s�񑀍�֌W
	//
	//===============================
	// �s��
	void SetMatrix(const RgMatrix& m, bool moveChild);
	// �s��̎擾
	const RgMatrix& GetMatrix()const { return m_mWorld; }

	//===============================
	//
	// ��{���̎擾�E�ݒ�
	//
	//===============================
	// ���O
	const std::string& IsName()const {
		return m_name;
	}
	void SetName(const std::string& name) { 
		m_name = name; 
	}

	//===============================
	//
	// �R���|�[�l���g�֌W
	//
	//===============================
	//
	void AddComponent(const std::shared_ptr<BaseComponent>& comp);

private:

	// ���O
	std::string m_name = "a";

	// �s��
	RgMatrix m_mWorld;		// ���[���h�s��
	RgMatrix m_mPrevWorld;	// �ύX�O�̍s��

	// �q���X�g
	std::list<std::shared_ptr<Object>>	m_children;
	// �e�|�C���^
	std::weak_ptr<Object>				m_parent;

	// �R���|�[�l���g
	std::list<std::shared_ptr<BaseComponent>> m_components;

	// �폜�t���O
	bool m_isDelete = false;
};

//
#include "System/DrawThreadManager.h"
//
#include "System/Level.h"


class GameManager
{
public:

	// ������
	bool Init();

	// ���
	void Release();

	// �`�揈��
	void Draw();

	// GUI����
	void ImGuiUpdate();

	// �Q�[������
	void Run();

	// ���ݑI�𒆂̃I�u�W�F�N�g
	std::weak_ptr<Object> m_Editor_Select_Object;

	EditorCamera m_EditorCamera;
	EditorStage m_EditorStage;


	// Component���X�g & ����
	std::map<std::string, std::function<std::shared_ptr<BaseComponent>()>>
		m_componentClassMaker;

private:

	// 
	std::shared_ptr<Level> m_level;

	//========================
	// �f�o�b�O�p
	//========================
	// ���f��
	std::shared_ptr<RgModel> m_model;
	std::shared_ptr<RgModel> m_model2;

	// �e�X�g�p
	RgPrimitive m_primitive;

	RgLineModel m_lineModel1;
	RgLineModel m_lineModel2;


	//===============================
	// �V���O���g���p�^�[��
	//===============================
private:

	//
	GameManager() {}

public:

	//
	static GameManager& GetInstance() {
		static GameManager instance;
		return instance;
	}
};
//
#define GAMEMGR GameManager::GetInstance()

// �N���X���[�J�[�̒Z�k
#define REGISTER_COMP_CLASS(className)\
GAMEMGR.m_componentClassMaker[#className] = []() {\
	return std::make_shared<className>();\
}